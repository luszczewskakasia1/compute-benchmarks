#pragma once

#include "framework/utility/command_line_argument.h"

#include <string>

struct TestCaseInterface {
    virtual ~TestCaseInterface() = default;
    virtual bool runFromCommandLine(CommandLineArguments &commandLineArguments) = 0;
    virtual std::string getHelp() const = 0;
    virtual std::string getHelpParameters() const = 0;
    virtual std::string getTestCaseName() const = 0;
};
