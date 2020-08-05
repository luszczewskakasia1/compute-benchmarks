#include "framework/test_case/test_map.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
#undef TEST_CASE
    return testMap;
}
