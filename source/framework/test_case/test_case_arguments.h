#pragma once

#include "framework/api.h"
#include "framework/test_case/test_case_argument.h"

#include <string>
#include <vector>

struct TestCaseArguments {
    bool parseArgument(const std::string &key, const std::string &value);
    bool validateArguments() const;
    std::string getHelp() const;
    std::string getCurrentConfig() const;

    std::vector<TestCaseArgument *> arguments;
    Api api = Api::Default;
    int iterations = 0;
};
