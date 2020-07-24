#pragma once

#include "framework/configuration.h"

#include <gtest/gtest.h>
#include <sstream>

class CustomEventListener : public ::testing::TestEventListener {
    void OnTestProgramStart(const ::testing::UnitTest &unitTest) override {
        if (::configuration.printType != Configuration::PrintType::Csv) {
            std::cout << "Running " << ::configuration.iterations << " iterations of each benchmark\n\n";
        }
    }
    void OnTestProgramEnd(const ::testing::UnitTest &unitTest) override {
        if (failsDetected) {
            std::cout << "\n"
                      << failMessage.str();
        }
    }

    void OnTestIterationStart(const ::testing::UnitTest &unitTest, int iteration) override {}
    void OnTestIterationEnd(const ::testing::UnitTest &unitTest, int iteration) override {}

    void OnEnvironmentsSetUpStart(const ::testing::UnitTest &unitTest) override {}
    void OnEnvironmentsSetUpEnd(const ::testing::UnitTest &unitTest) override {}

    void OnEnvironmentsTearDownStart(const ::testing::UnitTest &testCase) override {}
    void OnEnvironmentsTearDownEnd(const ::testing::UnitTest &testCase) override {}

    void OnTestCaseStart(const ::testing::TestCase &testCase) override {}
    void OnTestCaseEnd(const ::testing::TestCase &testCase) override {}

    void OnTestStart(const ::testing::TestInfo &testCase) override {}
    void OnTestEnd(const ::testing::TestInfo &testCase) override {
        if (testCase.result()->Failed()) {
            failMessage << "[  FAILED  ] " << testCase.test_case_name() << "." << testCase.name() << std::endl;
        }
    }

    void OnTestPartResult(const ::testing::TestPartResult &testPartResult) override {
        if (testPartResult.failed()) {
            failsDetected = true;
            std::cout << testPartResult.file_name() << ":" << testPartResult.line_number() << "\n"
                      << testPartResult.message() << "\n";
        }
    }

    std::ostringstream failMessage{};
    bool failsDetected = false;
};
