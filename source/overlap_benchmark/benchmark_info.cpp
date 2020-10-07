#include "framework/benchmark_info.h"

#include "overlap_benchmark/definitions/kernel_and_copy.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(KernelAndCopy)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "overlap_benchmark";
}

std::string getMeasurmentsUnit() {
    return "us";
}

int getTestCaseNameColumnWidth() {
    return 73;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
