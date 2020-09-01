#include "framework/benchmark_info.h"

#include "memory_benchmark/definitions/copy_buffer.h"
#include "memory_benchmark/definitions/fill_buffer.h"
#include "memory_benchmark/definitions/map_buffer.h"
#include "memory_benchmark/definitions/read_buffer.h"
#include "memory_benchmark/definitions/unmap_buffer.h"
#include "memory_benchmark/definitions/usm_copy.h"
#include "memory_benchmark/definitions/usm_fill.h"
#include "memory_benchmark/definitions/usm_memset.h"
#include "memory_benchmark/definitions/write_buffer.h"
#include "memory_benchmark/definitions/write_buffer_rect.h"

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
    TEST_CASE(WriteBufferRect)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "memory_benchmark";
}

std::string getMeasurmentsUnit() {
    return "GB/s";
}

int getTestCaseNameColumnWidth() {
    return 106;
}
