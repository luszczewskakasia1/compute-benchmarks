#include "framework/benchmark_info.h"

#include "ulls_benchmark/definitions/best_submission.h"
#include "ulls_benchmark/definitions/best_walker_submission.h"
#include "ulls_benchmark/definitions/best_walker_submission_immediate.h"
#include "ulls_benchmark/definitions/completion_latency.h"
#include "ulls_benchmark/definitions/empty_kernel.h"
#include "ulls_benchmark/definitions/kernel_with_work.h"
#include "ulls_benchmark/definitions/new_resources_submission_device.h"
#include "ulls_benchmark/definitions/new_resources_submission_host.h"
#include "ulls_benchmark/definitions/new_resources_with_gpu_access.h"
#include "ulls_benchmark/definitions/round_trip_submission.h"
#include "ulls_benchmark/definitions/usm_shared_first_cpu_access.h"
#include "ulls_benchmark/definitions/usm_shared_first_gpu_access.h"
#include "ulls_benchmark/definitions/walker_completion_latency.h"
#include "ulls_benchmark/definitions/walker_submission_events.h"
#include "ulls_benchmark/definitions/write_latency.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(BestWalkerSubmission)
    TEST_CASE(BestWalkerSubmissionImmediate)
    TEST_CASE(BestSubmission)
    TEST_CASE(EmptyKernel)
    TEST_CASE(KernelWithWork)
    TEST_CASE(NewResourcesSubmissionDevice)
    TEST_CASE(NewResourcesSubmissionHost)
    TEST_CASE(NewResourcesWithGpuAccess)
    TEST_CASE(RoundTripSubmission)
    TEST_CASE(CompletionLatency)
    TEST_CASE(UsmSharedFirstCpuAccess)
    TEST_CASE(UsmSharedFirstGpuAccess)
    TEST_CASE(WalkerCompletionLatency)
    TEST_CASE(WalkerSubmissionEvents)
    TEST_CASE(WriteLatency)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "ulls_benchmark";
}

std::string getBenchmarkDescription() {
    return "Ulls Benchmark is a set of tests aimed at measuring Ultra Low Latency Submission (ULLS) performance impact.";
}

std::string getMeasurmentsUnit() {
    return "us";
}

int getTestCaseNameColumnWidth() {
    return 60;
}

std::unique_ptr<BenchmarkSpecificConfigurationBase> BenchmarkSpecificConfigurationBase::create(TestCaseArgumentsBase &testCaseArguments) {
    return std::unique_ptr<BenchmarkSpecificConfigurationBase>{};
}
