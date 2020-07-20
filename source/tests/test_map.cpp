#include "test_map.h"

#include "tests/best_submission.h"
#include "tests/best_walker_submission.h"
#include "tests/empty_kernel.h"
#include "tests/new_resources_submission.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new UllsTest::name());
    TEST_CASE(BestWalkerSubmission)
    TEST_CASE(BestSubmission)
    TEST_CASE(EmptyKernel)
    TEST_CASE(NewResourcesSubmission)
#undef TEST_CASE
    return testMap;
}
