#pragma once

#include "framework/configuration.h"
#include "framework/test_case/test_case.h"

#include <unordered_map>

using TestMap = std::unordered_map<std::string, std::unique_ptr<TestCaseInterface>>;
TestMap getTestMap();

std::string getBenchmarkName();
inline std::string getBenchmarkFilename() {
#ifdef WIN32
    return getBenchmarkName() + ".exe";
#else
    return getBenchmarkName();
#endif
}

std::string getMeasurmentsUnit();

int getTestCaseNameColumnWidth();

struct BenchmarkSpecificConfigurationBase {
    static std::unique_ptr<BenchmarkSpecificConfigurationBase> create(TestCaseArgumentsBase &testCaseArguments);
};
