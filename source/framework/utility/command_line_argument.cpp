#include "command_line_argument.h"

#include "framework/utility/error.h"
#include "framework/utility/string_utils.h"

#include <unordered_set>

CommandLineArgument::CommandLineArgument(const char *token) {
    this->valid = parseArgumentToKeyValue(token, this->key, this->value);
}

bool CommandLineArgument::parseArguments(int argc, char **argv, std::vector<CommandLineArgument> &outArguments, std::string &outErrorMessage) {
    std::unordered_set<std::string> allKeys = {};

    for (int argIndex = 1; argIndex < argc; argIndex++) {
        CommandLineArgument arg{argv[argIndex]};

        // Validate syntax
        if (!arg.isValid()) {
            outErrorMessage = std::string("Argument \"") + argv[argIndex] + "\" is ill-formed. All arguments have to follow syntax: --<key>[=value]";
            outArguments.clear();
            return false;
        }

        // Check for duplicates
        const auto [iterator, inserted] = allKeys.insert(arg.getKey());
        if (!inserted) {
            outErrorMessage = std::string("Argument with a key \"") + arg.getKey() + "\" is provided more than once";
            outArguments.clear();
            return false;
        }

        outArguments.push_back(std::move(arg));
    }

    return true;
}

void CommandLineArgument::markAsProcessed() {
    ERROR_IF(!this->valid, "Invalid argument was marked as processed");
    ERROR_IF(this->processed, "Argument processed multiple times")
    this->processed = true;
}

bool CommandLineArgument::isKeyEqualTo(const std::string &key) const {
    if (!valid) {
        return false;
    }
    return this->key == key;
}
