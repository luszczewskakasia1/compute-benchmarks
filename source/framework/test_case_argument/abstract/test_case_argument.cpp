#include "test_case_argument.h"

#include "framework/test_case_argument/test_case_arguments.h"

#include <iomanip>

TestCaseArgument::TestCaseArgument(TestCaseArgumentsBase &parent, const std::string &key, const std::string &extraHelp)
    : key(key),
      extraHelp(extraHelp) {
    parent.arguments.push_back(this);
}

const std::string TestCaseArgument::getKey() const {
    return key;
}

std::string TestCaseArgument::getHelp() const {
    std::ostringstream result;
    if (!extraHelp.empty()) {
        const int maxSize = 25;
        result << std::left << std::setw(maxSize);
    }

    result << getHelpEntry(key);
    if (!extraHelp.empty()) {
        result << extraHelp;
    }
    return result.str();
}

std::string TestCaseArgument::getHelpEntry(const std::string &key) const {
    return std::string("--") + key + "=X ";
}
