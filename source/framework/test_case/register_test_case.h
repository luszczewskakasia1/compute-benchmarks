#pragma once

#include "framework/test_case/test_case.h"
template <typename TestCase>
struct RegisterTestCaseImplementation {
    explicit RegisterTestCaseImplementation(typename TestCase::BenchmarkImplementation::Function function, Api api, bool requiresIntelExtensions = false) {
        auto &implementation = TestCase::implementations[(int)api];
        implementation.function = function;
        implementation.requiresIntelExtensions = requiresIntelExtensions;
    }
};
