#include "framework/benchmark_info.h"

#include "pci_benchmark/map_buffer.h"
#include "pci_benchmark/read_buffer.h"
#include "pci_benchmark/usm_copy.h"
#include "pci_benchmark/usm_fill.h"
#include "pci_benchmark/usm_memset.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(MapBuffer)
    TEST_CASE(ReadBuffer)
    TEST_CASE(UsmCopy)
    TEST_CASE(UsmFill)
    TEST_CASE(UsmMemset)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "pci_benchmark";
}

std::string getMeasurmentsUnit() {
    return "GB/s";
}
