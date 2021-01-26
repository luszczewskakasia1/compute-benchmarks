#include "framework/l0/levelzero.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/affinity_mask_helper.h"
#include "framework/utility/process_group.h"

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

    // Get tiles for execution, validate if they are available
    std::vector<DeviceSelection> tilesForExecution = DeviceSelectionHelper::split(arguments.deviceSelection);
    for (DeviceSelection tile : tilesForExecution) {
        if (DeviceSelectionHelper::getSubDeviceIndex(tile) > tilesCount - 1) {
            return TestResult::DeviceNotCapable;
        }
    }
    const auto tilesForExecutionCount = tilesForExecution.size();
    for (auto i = 1u; i < arguments.processesPerTile; i++) {
        for (auto j = 0u; j < tilesForExecutionCount; j++) {
            tilesForExecution.push_back(tilesForExecution[j]);
        }
    }

    // Prepare processes
    ProcessGroup processes{"single_queue_workload_l0", tilesForExecution.size()};
    processes.addArgumentAll("iterations", std::to_string(arguments.iterations));
    processes.addArgumentAll("size", std::to_string(arguments.bufferSize));
    processes.addArgumentAll("synchronize", "1");
    for (auto i = 0u; i < processes.size(); i++) {
        const auto affinityMask = AffinityMaskHelper::createAffinityMask(Configuration::get().l0DeviceIndex, tilesForExecution[i]);
        processes[i].addEnvVariable("ZE_AFFINITY_MASK", affinityMask);
    }

    // Run processes
    processes.runAll();
    processes.synchronizeAll(arguments.iterations);
    processes.waitForFinishAll();
    if (TestResult result = processes.getResultAll(); result != TestResult::Success) {
        return result;
    }

    // Get results
    const std::vector<uint64_t> results = processes.getAverageMeasurementsAll(arguments.iterations);
    for (const auto result : results) {
        auto time = std::chrono::nanoseconds(result);
        time /= processes.size();
        statistics.pushValue(time);
    }

    return TestResult::Success;
}

static RegisterTestCaseImplementation<MultiProcessCompute> registerTestCase(run, Api::L0);
