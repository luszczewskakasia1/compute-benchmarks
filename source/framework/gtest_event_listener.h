#pragma once

#include "framework/configuration.h"
#include "framework/statistics.h"

#include <gtest/gtest.h>
#include <sstream>

class CustomEventListener : public ::testing::TestEventListener {
    struct ErrorInfo {
        ErrorInfo() : name(), errorMessage() {}
        std::ostringstream name;
        std::ostringstream errorMessage;
    } currentTestCaseErrorInfo{};

    void OnTestProgramStart(const ::testing::UnitTest &unitTest) override {
        if (::configuration.printType != Configuration::PrintType::Csv) {
            std::cout << "Running " << ::configuration.iterations << " iterations of each benchmark\n\n";
        }
        Statistics::printStatisticsHeader(::configuration.printType);
    }
    void OnTestProgramEnd(const ::testing::UnitTest &unitTest) override {
        if (errorInfos.size() > 0) {
            std::cout << "\n";
            for (const auto &errorInfo : errorInfos) {
                std::cout << "[  FAILED  ] " << errorInfo.name.str() << '\n'
                          << errorInfo.errorMessage.str() << '\n';
            }
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

    void OnTestStart(const ::testing::TestInfo &testCase) override {
        currentTestCaseErrorInfo = {};
    }
    void OnTestPartResult(const ::testing::TestPartResult &testPartResult) override {
        if (testPartResult.failed()) {
            currentTestCaseErrorInfo.errorMessage << testPartResult.file_name() << ":" << testPartResult.line_number() << "\n"
                                                  << testPartResult.message() << "\n";
        }
    }
    void OnTestEnd(const ::testing::TestInfo &testCase) override {
        if (testCase.result()->Failed()) {
            currentTestCaseErrorInfo.name << testCase.test_case_name() << "." << testCase.name();
            errorInfos.push_back(std::move(currentTestCaseErrorInfo));
        }
    }

    std::vector<ErrorInfo> errorInfos = {};
};
