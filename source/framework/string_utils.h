#pragma once

#include <string>

inline bool parseArgumentToKeyValue(const std::string &argument, std::string &outKey, std::string &outValue) {
    size_t index = argument.find('=');
    if (argument.find('=', index + 1) != std::string::npos) {
        return false;
    }

    outKey = argument.substr(0, index);
    outValue = argument.substr(index + 1);
    return true;
}
