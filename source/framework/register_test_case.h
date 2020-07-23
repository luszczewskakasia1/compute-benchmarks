#pragma once

#include "framework/test_case.h"
template <typename TestCase>
struct RegisterTestCase {
    explicit RegisterTestCase(typename TestCase::BenchmarkImplementation implementation, Api api) {
        TestCase::implementations[(int)api] = implementation;
    }
};
