#include "framework/utility/process.h"
#include "framework/utility/process_synchronization_helper.h"
#include "framework/utility/windows/windows.h"

#include <sstream>

struct ProcessDataWindows {
    // Resources to be freed
    PROCESS_INFORMATION processInfo{};
    struct ProcessPipes {
        HANDLE write = INVALID_HANDLE_VALUE;
        HANDLE read = INVALID_HANDLE_VALUE;
    };
    ProcessPipes processStdOut = {};
    ProcessPipes processStdIn = {};

    // Cached Values
    bool ended = false;
    bool hasResult = false;
    TestResult result = TestResult::Error;
    bool hasStdOut = false;
    std::string stdOut = {};
};

class EnvironmentRestorer {
  public:
    EnvironmentRestorer() {
        const char *environment = GetEnvironmentStringsA();
        size_t environmentSize = 0;
        while (environment[environmentSize] != '\0' || environment[environmentSize - 1] != '\0') {
            environmentSize++;
        }
        environmentSize++;

        this->storedEnv = std::make_unique<char[]>(environmentSize);
        std::memcpy(this->storedEnv.get(), environment, environmentSize);
        this->storedEnvSize = environmentSize;
    }

    ~EnvironmentRestorer() {
        SetEnvironmentStringsA(this->storedEnv.get());
    }

  private:
    std::unique_ptr<char[]> storedEnv = {};
    size_t storedEnvSize = {};
};

void Process::run() {
    auto processDataWindows = std::make_unique<ProcessDataWindows>();

    // Create pipes for stdout and stdin of the child process. Pipes that are passed to the process (stdOut.write, processStdIn.read)
    // should be closed after the process terminates.
    SECURITY_ATTRIBUTES pipeSecutrityAttributes;
    pipeSecutrityAttributes.nLength = sizeof(pipeSecutrityAttributes);
    pipeSecutrityAttributes.bInheritHandle = TRUE;
    pipeSecutrityAttributes.lpSecurityDescriptor = NULL;
    BOOL retVal = CreatePipe(&processDataWindows->processStdOut.read, &processDataWindows->processStdOut.write, &pipeSecutrityAttributes, 0);
    retVal = CreatePipe(&processDataWindows->processStdIn.read, &processDataWindows->processStdIn.write, &pipeSecutrityAttributes, 0);
    retVal = SetHandleInformation(processDataWindows->processStdOut.read, HANDLE_FLAG_INHERIT, 0);

    // Prepare arguments
    std::ostringstream commandLine = {};
    for (const auto &argument : this->arguments) {
        commandLine << argument.first;
        if (!argument.second.empty()) {
            commandLine << "=" << argument.second;
        }
        commandLine << ' ';
    }

    // Prepare env variables - create RAII restorer and update existing ones, so the child process inherits them
    EnvironmentRestorer envRestorer = {};
    for (const auto &envVariable : envVariables) {
        retVal = SetEnvironmentVariableA(envVariable.first.c_str(), envVariable.second.c_str());
    }

    // Prepare exeName (.exe extension is Windows-specific)
    const auto exeNameWithExtension = this->exeName + ".exe";

    // Start child process
    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(STARTUPINFO);
    startupInfo.hStdOutput = processDataWindows->processStdOut.write;
    startupInfo.hStdError = processDataWindows->processStdOut.write;
    startupInfo.hStdInput = processDataWindows->processStdIn.read;
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    PROCESS_INFORMATION processInfo{};
    auto result = CreateProcessA(
        exeNameWithExtension.c_str(),
        commandLine.str().data(),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startupInfo,
        &processDataWindows->processInfo);

    // Set process data
    this->osSpecificData = processDataWindows.release();
}

void Process::freeOsSpecificData() {
    ProcessDataWindows *processDataWindows = static_cast<ProcessDataWindows *>(this->osSpecificData);
    if (processDataWindows == nullptr) {
        return;
    }

    waitForFinish();

    BOOL retVal{};
    retVal = CloseHandle(processDataWindows->processInfo.hProcess);
    retVal = CloseHandle(processDataWindows->processInfo.hThread);
    retVal = CloseHandle(processDataWindows->processStdOut.read);
    retVal = CloseHandle(processDataWindows->processStdIn.write);

    delete processDataWindows;
    this->osSpecificData = nullptr;
}

void Process::waitForFinish() {
    ProcessDataWindows *processDataWindows = static_cast<ProcessDataWindows *>(this->osSpecificData);
    if (processDataWindows->ended) {
        return;
    }

    BOOL retVal = WaitForSingleObject(processDataWindows->processInfo.hProcess, std::numeric_limits<DWORD>::max());
    retVal = CloseHandle(processDataWindows->processStdOut.write);
    retVal = CloseHandle(processDataWindows->processStdIn.read);
    processDataWindows->ended = true;
}

TestResult Process::getResult() {
    ProcessDataWindows *processDataWindows = static_cast<ProcessDataWindows *>(this->osSpecificData);
    if (!processDataWindows->hasResult) {
        waitForFinish();

        DWORD exitCode{};
        BOOL retVal = GetExitCodeProcess(processDataWindows->processInfo.hProcess, &exitCode);

        processDataWindows->hasResult = true;
        processDataWindows->result = static_cast<TestResult>(exitCode);
    }
    return processDataWindows->result;
}

const std::string &Process::getStdout() {
    ProcessDataWindows *processDataWindows = static_cast<ProcessDataWindows *>(this->osSpecificData);
    if (!processDataWindows->hasStdOut) {
        waitForFinish();

        std::ostringstream output = {};
        const static size_t bufferSize = 1024u;
        CHAR buffer[bufferSize];
        while (true) {
            DWORD numberOfBytesRead = {};
            BOOL retVal = ReadFile(processDataWindows->processStdOut.read, buffer, bufferSize, &numberOfBytesRead, NULL);

            if (retVal == 0) {
                break;
            }
            if (numberOfBytesRead == 0) {
                break;
            }

            output << std::string{buffer, numberOfBytesRead};
        }

        processDataWindows->hasStdOut = true;
        processDataWindows->stdOut = output.str();
    }
    return processDataWindows->stdOut;
}

void Process::synchronizationSignal() {
    ProcessDataWindows *processDataWindows = static_cast<ProcessDataWindows *>(this->osSpecificData);

    char buffer = ProcessSynchronizationHelper::synchronizationChar;
    DWORD numberOfBytesWritten = {};
    BOOL retVal = WriteFile(processDataWindows->processStdIn.write, &buffer, 1, &numberOfBytesWritten, nullptr);
}

void Process::synchronizationWait() {
    ProcessDataWindows *processDataWindows = static_cast<ProcessDataWindows *>(this->osSpecificData);

    char buffer = {};
    DWORD numberOfBytesRead = {};
    BOOL retVal = ReadFile(processDataWindows->processStdOut.read, &buffer, 1, &numberOfBytesRead, NULL);
}
