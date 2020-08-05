#pragma once

#include <algorithm>
#include <string>

inline bool parseArgumentToKeyValue(const std::string &argument, std::string &outKey, std::string &outValue) {
    size_t index = argument.find('=');
    if (index == std::string::npos) {
        outKey = argument;
        outValue = "";
        return true;
    }

    if (argument.find('=', index + 1) != std::string::npos) {
        return false;
    }

    outKey = argument.substr(0, index);
    outValue = argument.substr(index + 1);
    return true;
}

inline std::string toLower(const std::string &arg) {
    std::string result = arg;
    std::transform(arg.begin(), arg.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}
