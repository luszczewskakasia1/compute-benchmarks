#include "test_case_arguments.h"

#include "framework/test_case_argument/test_case_argument.h"

#include <sstream>

bool TestCaseArgumentsBase::parseArgument(const std::string &key, const std::string &value) {
    for (auto &argument : arguments) {
        argument->parse(key, value);
    }
    return true;
}

bool TestCaseArgumentsBase::validateArguments() const {
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

std::string TestCaseArguments::getHelp() const {
    std::ostringstream result;
    for (const auto &argument : arguments) {
        result << "\t\t" << argument->getHelp() << '\n';
    }
    return result.str();
}

std::string TestCaseArguments::getCurrentConfig() const {
    std::ostringstream result;
    for (auto i = 0; i < arguments.size(); i++) {
        result << arguments[i]->toString();
        if (i != arguments.size() - 1) {
            result << " ";
        }
    }
    return result.str();
}

bool TestCaseArguments::validateArguments() const {
    if (!TestCaseArgumentsBase::validateArguments()) {
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
