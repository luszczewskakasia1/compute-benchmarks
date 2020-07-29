#include "framework/test_map.h"

#include "ulls_benchmark/best_submission.h"
#include "ulls_benchmark/best_walker_submission.h"
#include "ulls_benchmark/best_walker_submission_immediate.h"
#include "ulls_benchmark/completion_latency.h"
#include "ulls_benchmark/empty_kernel.h"
#include "ulls_benchmark/new_resources_submission_device.h"
#include "ulls_benchmark/new_resources_submission_host.h"
#include "ulls_benchmark/round_trip_submission.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new UllsTest::name());
    TEST_CASE(BestWalkerSubmission)
    TEST_CASE(BestWalkerSubmissionImmediate)
    TEST_CASE(BestSubmission)
    TEST_CASE(EmptyKernel)
    TEST_CASE(NewResourcesSubmissionDevice)
    TEST_CASE(NewResourcesSubmissionHost)
    TEST_CASE(RoundTripSubmission)
    TEST_CASE(CompletionLatency)
#undef TEST_CASE
    return testMap;
}
