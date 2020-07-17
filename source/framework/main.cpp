#include "tests/test_map.h"

#include <gtest/gtest.h>
#include <iostream>
#include <string>

int executeSingleTest(const std::string &testName, int argc, char **argv) {
    const TestMap testMap = getTestMap();
    auto it = testMap.find(testName);
    if (it == testMap.end()) {
        std::cerr << "Unknown test case\n";
        return 1;
    }

    TestCaseInterface *testCase = it->second.get();
    if (!testCase->runFromCommandLine(argc, argv)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }
    return 0;
}

int executeAllTests(int argc, char **argv) {
    std::cout << "executeAllTests\n";
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        const std::string singleTestParamName = "--test=";
        const std::string firstArgument{argv[1]};
        const auto position = firstArgument.find("--test=");
        if (position == 0) {
            std::string testName = firstArgument.substr(singleTestParamName.size());
            return executeSingleTest(testName, argc, argv);
        }
    }
    return executeAllTests(argc, argv);
}
