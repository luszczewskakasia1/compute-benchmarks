#pragma once

#include "framework/test_case/test_case_interface.h"

#include <map>
#include <memory>
#include <string>

struct TestMap : std::map<std::string, std::unique_ptr<TestCaseInterface>> {
    static TestMap &get() {
        static TestMap testMap = {};
        return testMap;
    }
};
