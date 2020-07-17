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

int printHelp() {
    std::cout << "ulls_benchmark.exe works in two modes.\n"
                 "\n\n"
                 "Default mode runs all available benchmarks in many predefined configurations. Underlying test engine is googletest, "
                 "so standard googletest arguments like --gtest_filter can be used, if necessary."
                 "\n\n"
                 "User can also specify one specific test case with custom parameter values. Running benchmarks in this fashion requires "
                 "using --test argument, followed by benchmark-specific parameters. Compute API (ocl or levelzero) can be selected with "
                 "the --api parameter. Example invocation:\n"
                 "\t.\\ulls_benchmark --test=UllsSubmissionWithEmptyKernel --api=ocl --workgroupSize=64 --workgroupCount=30"
                 "\n\n"
                 "Available test cases along are listed below:"
                 "\n";
    for (const auto &entry : getTestMap()) {
        std::cout << entry.second->getHelp() << "\n";
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        const std::string firstArgument{argv[1]};

        const std::string singleTestParamName = "--test=";
        if (firstArgument.find(singleTestParamName) == 0) {
            std::string testName = firstArgument.substr(singleTestParamName.size());
            return executeSingleTest(testName, argc, argv);
        }

        if (firstArgument == "-h" || firstArgument == "--help") {
            return printHelp();
        }
    }
    return executeAllTests(argc, argv);
}
