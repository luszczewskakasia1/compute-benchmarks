#pragma once

#include "framework/test_case/test_case.h"
template <typename TestCase>
struct RegisterTestCase {
    explicit RegisterTestCase(typename TestCase::BenchmarkImplementation::Function function, Api api, bool requiresIntelExtensions = false) {
        auto &implementation = TestCase::implementations[(int)api];
        implementation.function = function;
        implementation.requiresIntelExtensions = requiresIntelExtensions;
    }
};
