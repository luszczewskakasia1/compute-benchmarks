#pragma once

#include "framework/test_case/test_case.h"

#include <unordered_map>

using TestMap = std::unordered_map<std::string, std::unique_ptr<TestCaseInterface>>;
TestMap getTestMap();
