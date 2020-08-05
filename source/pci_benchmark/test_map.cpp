#include "framework/test_case/test_map.h"

#include "pci_benchmark/usm_copy.h"

TestMap getTestMap() {
    TestMap testMap;
#define TEST_CASE(name) testMap[#name] = std::unique_ptr<TestCaseInterface>(new name());
    TEST_CASE(UsmCopy)
#undef TEST_CASE
    return testMap;
}
