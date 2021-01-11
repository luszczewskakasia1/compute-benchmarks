#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/process.h"

#include "definitions/multi_process_compute.h"

#include <gtest/gtest.h>

static TestResult run(const MultiProcessComputeArguments &arguments, Statistics &statistics) {
    // Setup
    QueueProperties queueProperties = QueueProperties::create().disable();
    LevelZero levelzero{queueProperties};
    uint32_t tilesCount = {};
    ASSERT_ZE_RESULT_SUCCESS(zeDeviceGetSubDevices(levelzero.device, &tilesCount, nullptr));
    if (tilesCount == 0) {
        tilesCount = 1;
    }

    // Prepare processes to run
    std::vector<DeviceSelection> tilesForExecution = DeviceSelectionHelper::split(arguments.deviceSelection);
    std::vector<Process> processes = {};
    for (DeviceSelection tile : tilesForExecution) {
        if (DeviceSelectionHelper::getSubDeviceIndex(tile) > tilesCount - 1) {
            return TestResult::DeviceNotCapable;
        }

        processes.emplace_back("compute_workload_l0.exe");
        processes.back().addArgument("iterations", std::to_string(arguments.iterations));
        processes.back().addArgument("size", std::to_string(arguments.bufferSize));
    }

    // Run processes
    for (Process &process : processes) {
        process.run();
    }

    // Wait for all processes to end
    for (Process &process : processes) {
        const TestResult result = process.getResult();
        if (result != TestResult::Success) {
            return result;
        }
    }

    // Get results
    std::vector<uint64_t> results(arguments.iterations);
    for (Process &process : processes) {
        const auto stdOut = process.getStdout();
        const auto stdOutSplit = splitString(stdOut);
        for (auto resultIndex = 0u; resultIndex < stdOutSplit.size(); resultIndex++) {
            const auto result = std::atoll(stdOutSplit[resultIndex].c_str());
            results[resultIndex] += result;
        }
    }

    // Push average values to statistics
    for (auto result : results) {
        auto time = std::chrono::nanoseconds(result);
        time /= arguments.iterations;
        statistics.pushValue(time);
    }

    return TestResult::Success;
}

static RegisterTestCaseImplementation<MultiProcessCompute> registerTestCase(run, Api::L0);
