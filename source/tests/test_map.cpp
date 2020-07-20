#include "test_map.h"

#include "tests/best_walker_submission.h"
#include "tests/empty_kernel.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new UllsTest::name());
    TEST_CASE(BestWalkerSubmission)
    TEST_CASE(EmptyKernel)
#undef TEST_CASE
    return testMap;
}
