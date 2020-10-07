#include "framework/benchmark_info.h"

#include "api_overhead_benchmark/definitions/append_launch_kernel.h"
#include "api_overhead_benchmark/definitions/enqueue_ndr_null_lws.h"
#include "api_overhead_benchmark/definitions/enqueue_ndr_time.h"
#include "api_overhead_benchmark/definitions/execute_command_list.h"
#include "api_overhead_benchmark/definitions/flush_time.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(AppendLaunchKernel)
    TEST_CASE(EnqueueNdrNullLws)
    TEST_CASE(EnqueueNdrTime)
    TEST_CASE(ExecuteCommandList)
    TEST_CASE(FlushTime)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "api_overhead_benchmark";
}

std::string getMeasurmentsUnit() {
    return "us";
}

int getTestCaseNameColumnWidth() {
    return 68;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
