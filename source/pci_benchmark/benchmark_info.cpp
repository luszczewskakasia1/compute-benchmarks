#include "framework/benchmark_info.h"

#include "pci_benchmark/usm_copy.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(UsmCopy)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "pci_benchmark";
}

std::string getMeasurmentsUnit() {
    return "GB/s";
}
