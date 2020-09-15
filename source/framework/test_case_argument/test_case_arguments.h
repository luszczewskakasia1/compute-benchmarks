#pragma once

#include "framework/enum/api.h"

#include <string>
#include <vector>

struct TestCaseArgument;

struct TestCaseArguments {
    bool parseArgument(const std::string &key, const std::string &value);
    bool validateArguments() const;
    std::string getHelp() const;
    std::string getCurrentConfig() const;

    std::vector<TestCaseArgument *> arguments;
    Api api = Api::Unknown;
    int iterations = 0;
    bool noIntelExtensions = false;
};
