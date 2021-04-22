#pragma once

#include <algorithm>
#include <cctype>
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

inline bool containsIllegalCharacters(const std::string &string, const std::string &illegalCharacters) {
    const auto predicate = [illegalCharacters](char c) {
        return illegalCharacters.find(c) != std::string::npos;
    };
    return std::any_of(string.begin(), string.end(), predicate);
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

template <typename T>
using ToStringConverter = std::string (*)(T);

template <typename T>
inline std::string joinStrings(const std::string &separator, const std::vector<T> &objects, ToStringConverter<T> toString) {
    std::ostringstream result{};
    for (int i = 0; i < objects.size(); i++) {
        result << toString(objects[i]);
        if (i < objects.size() - 1) {
            result << separator;
        }
    }
    return result.str();
}
