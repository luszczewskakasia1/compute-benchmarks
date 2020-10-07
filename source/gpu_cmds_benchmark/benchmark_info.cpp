#include "framework/benchmark_info.h"

#include "gpu_cmds_benchmark/definitions/empty_kernel.h"
#include "gpu_cmds_benchmark/definitions/kernel_with_work.h"
#include "gpu_cmds_benchmark/definitions/wait_on_event_cold.h"
#include "gpu_cmds_benchmark/definitions/wait_on_event_hot.h"
#include "gpu_cmds_benchmark/definitions/write_timestamp.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(EmptyKernel)
    TEST_CASE(KernelWithWork)
    TEST_CASE(WaitOnEventCold)
    TEST_CASE(WaitOnEventHot)
    TEST_CASE(WriteTimestamp)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "gpu_cmds_benchmark";
}

std::string getMeasurmentsUnit() {
    return "us";
}

int getTestCaseNameColumnWidth() {
    return 77;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
