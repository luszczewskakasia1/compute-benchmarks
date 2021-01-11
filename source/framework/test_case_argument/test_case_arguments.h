#pragma once

#include "framework/enum/api.h"
#include "framework/utility/command_line_argument.h"

#include <string>
#include <vector>

struct TestCaseArgument;

struct TestCaseArgumentsBase {
    bool parseArgument(CommandLineArgument &commandLineArgument);
    bool parseArguments(CommandLineArguments &commandLineArguments);
    virtual bool validateArguments() const;
    std::string getHelp(size_t indent) const;

    std::vector<const TestCaseArgument *> getUnparsedArguments() const;

    std::vector<TestCaseArgument *> arguments;

  protected:
    virtual bool validateArgumentsExtra() const { return true; } // This is optional. Use this for validating dependencies between arguments if any.
};

struct TestCaseArguments : TestCaseArgumentsBase {

    std::string getCurrentConfig(bool commandLine) const;
    bool validateArguments() const override;

    Api api = Api::Unknown;
    size_t iterations = 0;
    bool noIntelExtensions = false;
    bool isSingleTestMode = false;
};
