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

#include "framework/test_case/test_case.h"
#include "framework/test_map.h"
#include "framework/utility/string_utils.h"

template <typename TestCase>
struct RegisterTestCaseImplementation {
    explicit RegisterTestCaseImplementation(typename TestCase::BenchmarkImplementation::Function function, Api api, bool requiresIntelExtensions = false) {
        auto &implementation = TestCase::implementations[(int)api];
        implementation.function = function;
        implementation.requiresIntelExtensions = requiresIntelExtensions;
    }
};

template <typename ConcreteTestCase>
struct RegisterTestCase {
    explicit RegisterTestCase() {
        using ExpectedBaseClass = TestCase<typename ConcreteTestCase::ArgumentContainerT>;
        static_assert(std::is_base_of_v<ExpectedBaseClass, ConcreteTestCase>, "ConcreteTestCase should derive from TestCase");
        TestMap &testMap = TestMap::get();

        auto testCase = std::unique_ptr<TestCaseInterface>(new ConcreteTestCase());
        DEVELOPER_WARNING_IF(testMap.find(testCase->getTestCaseName()) != testMap.end(), "Multiple tests have a name \"", testCase->getTestCaseName(), "\"");
        DEVELOPER_WARNING_IF(testCase->getTestCaseName().empty(), "Test case with empty name detected");
        DEVELOPER_WARNING_IF(containsIllegalCharacters(testCase->getTestCaseName(), " -:='\""), "Test case name \"", testCase->getTestCaseName(), "\" contains illegal characters");

        TestMap::get()[testCase->getTestCaseName()] = std::move(testCase);
    }
};
