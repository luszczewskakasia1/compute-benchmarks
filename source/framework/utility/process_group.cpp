#include "process_group.h"

#include "framework/utility/error.h"
#include "framework/utility/string_utils.h"

ProcessGroup::ProcessGroup(const std::string &binaryName, size_t count)
    : binaryName(binaryName) {
    for (auto processIndex = 0; processIndex < count; processIndex++) {
        processes.emplace_back(binaryName);
    }
}

void ProcessGroup::addArgumentAll(const std::string &key, const std::string &value) {
    for (Process &process : processes) {
        process.addArgument(key, value);
    }
}

void ProcessGroup::addEnvVariableAll(const std::string &key, const std::string &value) {
    for (Process &process : processes) {
        process.addEnvVariable(key, value);
    }
}

void ProcessGroup::runAll() {
    for (Process &process : processes) {
        process.run();
    }
}

void ProcessGroup::synchronizeAll(size_t iterationsCount) {
    for (auto iteration = 0u; iteration < iterationsCount; iteration++) {
        for (Process &process : processes) {
            process.synchronizationWait();
        }

        for (Process &process : processes) {
            process.synchronizationSignal();
        }
    }
}

void ProcessGroup::waitForFinishAll() {
    for (Process &process : processes) {
        process.waitForFinish();
    }
}

TestResult ProcessGroup::getResultAll() {
    for (Process &process : processes) {
        const auto result = process.getResult();
        if (result != TestResult::Success) {
            return result;
        }
    }
    return TestResult::Success;
}

std::vector<std::vector<uint64_t>> ProcessGroup::getMeasurementsAll(size_t expectedCount) {
    std::vector<std::vector<uint64_t>> result = {};
    result.reserve(processes.size());

    for (auto processIndex = 0; processIndex < processes.size(); processIndex++) {
        const auto stdOut = processes[processIndex].getStdout();
        const auto stdOutSplit = splitString(stdOut);
        ERROR_IF(stdOutSplit.size() != expectedCount, "Child process returned an invalid number of measurements");

        std::vector<uint64_t> measurementsFromProcess = {};
        for (const auto measurementString : stdOutSplit) {
            const auto measurement = std::atoll(measurementString.c_str());
            measurementsFromProcess.push_back(measurement);
        }

        result.push_back(std::move(measurementsFromProcess));
    }

    return result;
}

std::vector<uint64_t> ProcessGroup::getAverageMeasurementsAll(size_t expectedCount) {
    std::vector<uint64_t> result(expectedCount);
    for (const auto &measurementsFromProcess : getMeasurementsAll(expectedCount)) {
        for (auto i = 0u; i < measurementsFromProcess.size(); i++) {
            result[i] += measurementsFromProcess[i];
        }
    }
    return result;
}

Process &ProcessGroup::operator[](size_t index) {
    ERROR_IF(index >= processes.size(), "Invalid process index");
    return processes[index];
}

size_t ProcessGroup::size() const {
    return processes.size();
}
