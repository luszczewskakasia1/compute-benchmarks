#pragma once

#include "framework/enum/api.h"

#include <string>
#include <vector>

struct TestCaseArgument;

struct TestCaseArgumentsBase {
    bool parseArgument(const std::string &key, const std::string &value);
    virtual bool validateArguments() const;

    std::vector<TestCaseArgument *> arguments;

  protected:
    virtual bool validateArgumentsExtra() const { return true; } // This is optional. Use this for validating dependencies between arguments if any.
};

struct TestCaseArguments : TestCaseArgumentsBase {
    std::string getHelp() const;
    std::string getCurrentConfig() const;
    bool validateArguments() const override;

    Api api = Api::Unknown;
    size_t iterations = 0;
    bool noIntelExtensions = false;
};
