#pragma once

#include "framework/enum/api.h"
#include "framework/utility/command_line_argument.h"

#include <string>
#include <vector>

struct Argument;

struct ArgumentContainer {
    bool parseArgument(CommandLineArgument &commandLineArgument);
    bool parseArguments(CommandLineArguments &commandLineArguments);
    virtual bool validateArguments() const;
    std::string getHelp(size_t indent) const;

    std::vector<const Argument *> getUnparsedArguments() const;

    std::vector<Argument *> arguments;

  protected:
    virtual bool validateArgumentsExtra() const { return true; } // This is optional. Use this for validating dependencies between arguments if any.
};
