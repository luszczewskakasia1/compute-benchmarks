#include "framework/utility/error.h"
#include "framework/utility/process.h"
#include "framework/utility/process_synchronization_helper.h"

#include <memory>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static std::string getErrorFromErrno() {
    std::ostringstream result{};
    result << "errno=" << errno;
    result << " (" << strerror(errno) << ")";
    return result.str();
}

#define ERROR_IF_SYS_CALL_FAILED(call, message) ERROR_IF(call < 0, std::string(message) + ", " + getErrorFromErrno());

struct ProcessDataLinux {
    struct ProcessPipes {
        int pipes[2];
        int &read() { return pipes[0]; }
        int &write() { return pipes[1]; }
    };
    ProcessPipes processStdOut = {};
    ProcessPipes processStdIn = {};

    pid_t childPid = {};
    bool ended = false;
    TestResult result = TestResult::Error;
    bool hasStdOut = false;
    std::string stdOut = {};
};

void Process::run() {
    auto processDataLinux = std::make_unique<ProcessDataLinux>();

    // Create pipes for stdout and stdin of the child process.
    ERROR_IF_SYS_CALL_FAILED(pipe(processDataLinux->processStdOut.pipes), "Creating pipe failed, ");
    ERROR_IF_SYS_CALL_FAILED(pipe(processDataLinux->processStdIn.pipes), "Creating pipe failed, ");

    // Fork the process
    processDataLinux->childPid = fork();
    ERROR_IF(processDataLinux->childPid == -1, "Creating process failed");

    // Different behaviour for parent and slave process
    if (processDataLinux->childPid != 0) {
        // We're in parent process

        // Close pipes that we won't need (these are descriptors, which will be used by child)
        ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdIn.read()), "closing pipe failed");
        ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdOut.write()), "closing pipe failed");

        // Store all data in Process class
        this->osSpecificData = processDataLinux.release();
    } else {
        // Override current stdout and stdin in with pipes connected to parent
        ERROR_IF_SYS_CALL_FAILED(dup2(processDataLinux->processStdIn.read(), STDIN_FILENO), "dup2 for stdin failed");
        ERROR_IF_SYS_CALL_FAILED(dup2(processDataLinux->processStdOut.write(), STDOUT_FILENO), "dup2 for stdout failed");

        // Close unneeded pipes
        ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdOut.read()), "closing pipe failed");
        ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdOut.write()), "closing pipe failed");
        ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdIn.read()), "closing pipe failed");
        ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdIn.write()), "closing pipe failed");

        // Prepare arguments
        std::vector<std::string> argumentsForExecStrings = {};
        argumentsForExecStrings.reserve(this->arguments.size());
        for (auto &argument : this->arguments) {
            std::string str = argument.first;
            if (!argument.second.empty()) {
                str += "=";
                str += argument.second;
            }
            argumentsForExecStrings.push_back(std::move(str));
        }
        std::vector<char *> argumentsForExec = {};
        argumentsForExec.reserve(argumentsForExec.size() + 1);
        for (auto &argumentsForExecString : argumentsForExecStrings) {
            argumentsForExec.push_back(argumentsForExecString.data());
        }
        argumentsForExec.push_back(nullptr);

        // Prepare environment
        std::vector<std::string> environmentForExecStrings = {};
        environmentForExecStrings.reserve(this->envVariables.size());
        for (auto &envVariable : this->envVariables) {
            std::string str = envVariable.first;
            if (!envVariable.second.empty()) {
                str += "=";
                str += envVariable.second;
            }
            environmentForExecStrings.push_back(std::move(str));
        }
        std::vector<char *> environmentForExec = {};
        environmentForExec.reserve(environmentForExecStrings.size() + 1);
        for (auto &environmentForExecString : environmentForExecStrings) {
            environmentForExec.push_back(environmentForExecString.data());
        }
        environmentForExec.push_back(nullptr);

        // Load new binary image
        const int execResult = execve(this->exeName.c_str(), argumentsForExec.data(), environmentForExec.data());
        ERROR_IF_SYS_CALL_FAILED(execResult, "Sys call execve failed, ");
        ERROR("Unreachable code after execve");
    }
}

void Process::freeOsSpecificData() {
    ProcessDataLinux *processDataLinux = static_cast<ProcessDataLinux *>(this->osSpecificData);
    if (processDataLinux == nullptr) {
        return;
    }

    ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdIn.write()), "closing pipe failed");
    ERROR_IF_SYS_CALL_FAILED(close(processDataLinux->processStdOut.read()), "closing pipe failed");
    delete processDataLinux;
}

void Process::waitForFinish() {
    ProcessDataLinux *processDataLinux = static_cast<ProcessDataLinux *>(this->osSpecificData);
    if (processDataLinux->ended) {
        return;
    }

    while (true) {
        int status{};
        int pid = waitpid(processDataLinux->childPid, &status, 0);
        ERROR_IF(pid == -1, std::string("waitpid() returned an error, ") + getErrorFromErrno());
        ERROR_IF(pid != processDataLinux->childPid, "waitpid() signalled from wrong child process");
        ERROR_IF(WIFSIGNALED(status), "child process killed by signal")
        ERROR_IF(WIFSTOPPED(status), "child process stopped by signal")

        if (WIFEXITED(status)) {
            processDataLinux->result = static_cast<TestResult>(WEXITSTATUS(status));
            break;
        }
    }

    processDataLinux->ended = true;
}

TestResult Process::getResult() {
    waitForFinish();
    ProcessDataLinux *processDataLinux = static_cast<ProcessDataLinux *>(this->osSpecificData);
    return processDataLinux->result;
}

const std::string &Process::getStdout() {
    waitForFinish();
    ProcessDataLinux *processDataLinux = static_cast<ProcessDataLinux *>(this->osSpecificData);
    if (!processDataLinux->hasStdOut) {
        waitForFinish();

        std::ostringstream output{};
        const static size_t bufferSize = 1024u;
        char buffer[bufferSize];
        while (true) {
            ssize_t numberOfBytesRead = read(processDataLinux->processStdOut.read(), buffer, bufferSize);
            ERROR_IF_SYS_CALL_FAILED(numberOfBytesRead, "reading a child process stdOut failed");

            if (numberOfBytesRead == 0) {
                break;
            }

            output << std::string{buffer, static_cast<size_t>(numberOfBytesRead)};
        }

        processDataLinux->hasStdOut = true;
        processDataLinux->stdOut = output.str();
    }

    return processDataLinux->stdOut;
}

void Process::synchronizationSignal() {
    ProcessDataLinux *processDataLinux = static_cast<ProcessDataLinux *>(this->osSpecificData);

    char buffer = ProcessSynchronizationHelper::synchronizationChar;
    ssize_t numberOfBytesWritten = write(processDataLinux->processStdIn.write(), &buffer, 1);
    ERROR_IF_SYS_CALL_FAILED(numberOfBytesWritten, "reading a child process stdOut failed");
    ERROR_IF(numberOfBytesWritten == 0, "No character was written when waiting on a child process");
}

void Process::synchronizationWait() {
    ProcessDataLinux *processDataLinux = static_cast<ProcessDataLinux *>(this->osSpecificData);

    char buffer = {};
    ssize_t numberOfBytesRead = read(processDataLinux->processStdOut.read(), &buffer, 1u);
    ERROR_IF_SYS_CALL_FAILED(numberOfBytesRead, "reading a child process stdOut failed");
    ERROR_IF(numberOfBytesRead == 0, "No character was read when waiting on a child process");
    ERROR_IF(buffer != ProcessSynchronizationHelper::synchronizationChar, "invalid synchronization character detected");
}
