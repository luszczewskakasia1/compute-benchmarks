#include "test_case_argument.h"

#include "framework/test_case_argument/test_case_arguments.h"
#include "framework/utility/command_line_argument.h"

#include <iomanip>

Argument::Argument(ArgumentsBase &parent, const std::string &key, const std::string &extraHelp)
    : key(key),
      extraHelp(extraHelp) {
    parent.arguments.push_back(this);
}

const std::string Argument::getKey() const {
    return key;
}

std::string Argument::getHelp() const {
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

void Argument::parse(CommandLineArgument &argument) {
    if (argument.isKeyEqualTo(this->key)) {
        argument.markAsProcessed();
        parseImpl(argument.getValue());
        this->parsed = true;
    }
}

std::string Argument::getHelpEntry(const std::string &key) const {
    return std::string("--") + key + "=X ";
}
