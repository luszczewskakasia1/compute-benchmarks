#include "framework/benchmark_info.h"

#include "api_overhead_benchmark/definitions/enqueue_ndr_null_lws.h"
#include "api_overhead_benchmark/definitions/enqueue_ndr_time.h"
#include "api_overhead_benchmark/definitions/flush_time.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(EnqueueNdrNullLws)
    TEST_CASE(EnqueueNdrTime)
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
    return 60;
}
