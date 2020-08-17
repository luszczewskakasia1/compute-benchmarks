#include "framework/benchmark_info.h"

#include "pci_benchmark/copy_buffer.h"
#include "pci_benchmark/fill_buffer.h"
#include "pci_benchmark/map_buffer.h"
#include "pci_benchmark/read_buffer.h"
#include "pci_benchmark/unmap_buffer.h"
#include "pci_benchmark/usm_copy.h"
#include "pci_benchmark/usm_fill.h"
#include "pci_benchmark/usm_memset.h"
#include "pci_benchmark/write_buffer.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(CopyBuffer)
    TEST_CASE(FillBuffer)
    TEST_CASE(MapBuffer)
    TEST_CASE(ReadBuffer)
    TEST_CASE(UnmapBuffer)
    TEST_CASE(UsmCopy)
    TEST_CASE(UsmFill)
    TEST_CASE(UsmMemset)
    TEST_CASE(WriteBuffer)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "pci_benchmark";
}

std::string getMeasurmentsUnit() {
    return "GB/s";
}
