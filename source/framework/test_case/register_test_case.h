#pragma once

#include "framework/benchmark_info.h"
#include "framework/test_case/test_case.h"

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
        using ExpectedBaseClass = TestCase<typename ConcreteTestCase::Arguments>;
        static_assert(std::is_base_of_v<ExpectedBaseClass, ConcreteTestCase>, "ConcreteTestCase should derive from TestCase");

        auto testCase = std::unique_ptr<TestCaseInterface>(new ConcreteTestCase());
        getTestMap()[testCase->getTestCaseName()] = std::move(testCase);
    }
};
