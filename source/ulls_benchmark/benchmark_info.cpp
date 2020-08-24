#include "framework/benchmark_info.h"

#include "ulls_benchmark/best_submission.h"
#include "ulls_benchmark/best_walker_submission.h"
#include "ulls_benchmark/best_walker_submission_immediate.h"
#include "ulls_benchmark/completion_latency.h"
#include "ulls_benchmark/empty_kernel.h"
#include "ulls_benchmark/new_resources_submission_device.h"
#include "ulls_benchmark/new_resources_submission_host.h"
#include "ulls_benchmark/round_trip_submission.h"
#include "ulls_benchmark/walker_completion_latency.h"
#include "ulls_benchmark/walker_submission_events.h"
#include "ulls_benchmark/write_latency.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(BestWalkerSubmission)
    TEST_CASE(BestWalkerSubmissionImmediate)
    TEST_CASE(BestSubmission)
    TEST_CASE(EmptyKernel)
    TEST_CASE(NewResourcesSubmissionDevice)
    TEST_CASE(NewResourcesSubmissionHost)
    TEST_CASE(RoundTripSubmission)
    TEST_CASE(CompletionLatency)
    TEST_CASE(WalkerCompletionLatency)
    TEST_CASE(WalkerSubmissionEvents)
    TEST_CASE(WriteLatency)
#undef TEST_CASE
    return testMap;
}

std::string getBenchmarkName() {
    return "ulls_benchmark";
}

std::string getMeasurmentsUnit() {
    return "us";
}

int getTestCaseNameColumnWidth() {
    return 49;
}
