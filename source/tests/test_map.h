#pragma once

#include "framework/test_case.h"
#include "tests/ulls_best_submission_with_walker.h"
#include "tests/ulls_submission_with_empty_kernel.h"

#include <unordered_map>

using TestMap = std::unordered_map<std::string, std::unique_ptr<TestCaseInterface>>;
inline TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(UllsBestSubmissionWithWalker)
    TEST_CASE(UllsSubmissionWithEmptyKernel)
#undef TEST_CASE
    return testMap;
}
