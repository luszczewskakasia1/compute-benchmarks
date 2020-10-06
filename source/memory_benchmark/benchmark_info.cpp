#include "memory_benchmark/benchmark_specific_configuration.h"
#include "memory_benchmark/definitions/copy_buffer.h"
#include "memory_benchmark/definitions/fill_buffer.h"
#include "memory_benchmark/definitions/map_buffer.h"
#include "memory_benchmark/definitions/read_buffer.h"
#include "memory_benchmark/definitions/unmap_buffer.h"
#include "memory_benchmark/definitions/usm_copy.h"
#include "memory_benchmark/definitions/usm_fill.h"
#include "memory_benchmark/definitions/usm_fill_specific_pattern.h"
#include "memory_benchmark/definitions/usm_memset.h"
#include "memory_benchmark/definitions/usm_shared_migrate_cpu.h"
#include "memory_benchmark/definitions/usm_shared_migrate_gpu.h"
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
    TEST_CASE(UsmFillSpecificPattern)
    TEST_CASE(UsmMemset)
    TEST_CASE(UsmSharedMigrateCpu)
    TEST_CASE(UsmSharedMigrateGpu)
    TEST_CASE(WriteBuffer)
    TEST_CASE(WriteBufferRect)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "memory_benchmark";
}

std::string getMeasurmentsUnit() {
    auto configuration = static_cast<BenchmarkSpecificConfiguration *>(::configuration.benchmarkSpecificConfiguration);
    if (configuration->printBandwidth) {
        return "GB/s";
    } else {
        return "us";
    }
}

int getTestCaseNameColumnWidth() {
    return 106;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create() {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{new BenchmarkSpecificConfiguration()};
}
