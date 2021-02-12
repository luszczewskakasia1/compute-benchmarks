#include "arguments.h"

#include "framework/argument/abstract/argument.h"

#include <sstream>

bool ArgumentsBase::parseArgument(CommandLineArgument &commandLineArgument) {
    for (auto &argument : arguments) {
        argument->parse(commandLineArgument);
    }
    return true;
}

bool ArgumentsBase::parseArguments(CommandLineArguments &commandLineArguments) {
    for (CommandLineArgument &commandLineArgument : commandLineArguments) {
        if (!this->parseArgument(commandLineArgument)) {
            return false;
        }
    }
    return true;
}

bool ArgumentsBase::validateArguments() const {
    for (const auto &argument : arguments) {
        if (!argument->validate()) {
            return false;
        }
    }

    if (!validateArgumentsExtra()) {
        return false;
    }

    return true;
}

std::string ArgumentsBase::getHelp(size_t indent) const {
    std::ostringstream result;
    for (const auto &argument : arguments) {
        for (auto i = 0u; i < indent; i++) {
            result << '\t';
        }
        result << argument->getHelp() << '\n';
    }
    return result.str();
}

std::string Arguments::getCurrentConfig(bool commandLine) const {
    std::ostringstream result;
    for (auto i = 0; i < arguments.size(); i++) {
        if (commandLine) {
            result << "--";
        }
        result << arguments[i]->toString();
        if (i != arguments.size() - 1) {
            result << " ";
        }
    }
    return result.str();
}

std::vector<const Argument *> ArgumentsBase::getUnparsedArguments() const {
    std::vector<const Argument *> result = {};
    for (const auto &argument : arguments) {
        if (!argument->wasParsed()) {
            result.push_back(argument);
        }
    }
    return result;
}

bool Arguments::validateArguments() const {
    if (!ArgumentsBase::validateArguments()) {
        return false;
    }

    if (!validateApi(api)) {
        return false;
    }

    if (iterations <= 0) {
        return false;
    }

    return true;
}
