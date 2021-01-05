#pragma once

#include "framework/test_case/test_case_interface.h"

#include <memory>
#include <string>
#include <unordered_map>

struct TestMap : std::unordered_map<std::string, std::unique_ptr<TestCaseInterface>> {
    static TestMap &get() {
        static TestMap testMap = {};
        return testMap;
    }
};
