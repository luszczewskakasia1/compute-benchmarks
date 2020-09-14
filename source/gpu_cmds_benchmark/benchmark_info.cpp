#include "framework/benchmark_info.h"

#include "gpu_cmds_benchmark/definitions/wait_on_event_hot.h"
#include "gpu_cmds_benchmark/definitions/write_timestamp.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(WriteTimestamp)
    TEST_CASE(WaitOnEventHot)
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
    return 60;
}
