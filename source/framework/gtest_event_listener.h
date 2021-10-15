/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include "framework/benchmark_info.h"
#include "framework/configuration.h"
#include "framework/test_case/test_case_statistics.h"

#include <gtest/gtest.h>
#include <sstream>

class AllTestsGtestListener : public ::testing::EmptyTestEventListener {
    struct ErrorInfo {
        ErrorInfo() : name(), errorMessage() {}
        std::ostringstream name;
        std::ostringstream errorMessage;
    } currentTestCaseErrorInfo{};

    void OnTestProgramStart([[maybe_unused]] const ::testing::UnitTest &unitTest) override {
        if (!Configuration::get().noHeaders && Configuration::get().printType != Configuration::PrintType::Csv) {
            std::cout << "Running " << Configuration::get().iterations << " iterations of each benchmark\n\n";
        }
        if (!Configuration::get().noColumnNames) {
            TestCaseStatistics::printStatisticsHeader(Configuration::get().printType);
        }
    }
    void OnTestProgramEnd([[maybe_unused]] const ::testing::UnitTest &unitTest) override {
        dumpErrors();
    }

    void OnTestStart([[maybe_unused]] const ::testing::TestInfo &testCase) override {
        currentTestCaseErrorInfo = {};
    }
    void OnTestPartResult(const ::testing::TestPartResult &testPartResult) override {
        if (testPartResult.failed()) {
            currentTestCaseErrorInfo.errorMessage << testPartResult.message() << "\n";
        }
    }
    void OnTestEnd(const ::testing::TestInfo &testCase) override {
        if (testCase.result()->Failed()) {
            currentTestCaseErrorInfo.name << testCase.test_case_name() << "." << testCase.name();
            errorInfos.push_back(std::move(currentTestCaseErrorInfo));
        }
        if (Configuration::get().dumpErrorsImmediately) {
            dumpErrors();
        }
    }

    void dumpErrors() {
        if (errorInfos.size() > 0) {
            std::cout << "\n";
            for (const auto &errorInfo : errorInfos) {
                std::cout << "[  FAILED  ] " << errorInfo.name.str() << '\n'
                          << errorInfo.errorMessage.str() << '\n';
            }
        }
        errorInfos.clear();
    }

    std::vector<ErrorInfo> errorInfos = {};
};

class SingleTestGtestListener : public ::testing::EmptyTestEventListener {
    void OnTestPartResult(const ::testing::TestPartResult &testPartResult) override {
        if (testPartResult.failed()) {
            std::cout << "\n"
                      << testPartResult.message()
                      << "\n";
        }
    }
};

template <typename ListenerType>
void replaceGtestListener() {
    auto &listeners = ::testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new ListenerType());
}
