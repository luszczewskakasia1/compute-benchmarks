#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

inline bool parseArgumentToKeyValue(const std::string &argument, std::string &outKey, std::string &outValue) {
    // Check for common errors
    const std::string prefix = "--";
    const bool incorrectPrefix = argument.find(prefix) != 0;
    const bool empty = argument == prefix;
    if (incorrectPrefix || empty) {
        return false;
    }

    // Find '=', which is boundary between key and value. If it's not present, treat argument as a flag, which is also valid
    size_t index = argument.find('=');
    if (index == std::string::npos) {
        outKey = argument.substr(prefix.size());
        outValue = "";
        return true;
    }

    // Key-Value argument, also valid
    outKey = argument.substr(prefix.size(), index - prefix.size());
    outValue = argument.substr(index + 1);
    return true;
}

inline std::string toLower(const std::string &arg) {
    std::string result = arg;
    std::transform(arg.begin(), arg.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

inline std::vector<std::string> splitString(const std::string &string) {
    std::vector<std::string> result = {};
    std::istringstream stringStream(string);

    std::string token{};
    while (stringStream >> token) {
        result.push_back(token);
    }

    return result;
}
