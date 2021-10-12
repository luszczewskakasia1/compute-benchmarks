/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

template <typename... Args>
inline void printToOstream(std::ostream &stream, Args &&... args) {
    (stream << ... << args);
}

template <typename... Args>
inline void printMessageLine(const char *label, Args &&... args) {
    static_assert(sizeof...(args) > 0, "A textual message of the FATAL_ERROR is required");
    printToOstream(std::cerr, label, ": ", std::forward<Args>(args)...);
    std::cerr << std::endl;
}

inline std::string composeErrorStringForMacro(const char *file, int line, const char *macroName, const char *macroArg, const char *macroArgValue, const char *macroArgValueEnum) {
    std::ostringstream result{};
    result << "FAILED assertion " << macroName << '(' << macroArg << ")\n";
    result << "\tvalue: " << macroArgValue;
    if (macroArgValueEnum) {
        result << " (" << macroArgValueEnum << ')';
    }
    result << '\n';
    result << "\tLocation: " << file << ':' << line;
    return result.str();
}

#define NON_FATAL_ERROR(macroName, macroArg, macroArgValue, macroArgValueEnum) \
    GTEST_NONFATAL_FAILURE_(composeErrorStringForMacro(__FILE__, __LINE__, macroName, macroArg, macroArgValue, macroArgValueEnum).c_str());

#define FATAL_ERROR(...)                    \
    printMessageLine("ERROR", __VA_ARGS__); \
    throw std::exception();

#define FATAL_ERROR_IF(condition, ...) \
    if (condition) {                   \
        FATAL_ERROR(__VA_ARGS__)       \
    }

#define FATAL_ERROR_UNLESS(condition, message) FATAL_ERROR_IF(!(condition), (message))

#define FATAL_ERROR_IN_DESTRUCTOR(...)      \
    printMessageLine("ERROR", __VA_ARGS__); \
    std::abort();

#define DEVELOPER_WARNING(...) \
    printMessageLine("DEVELOPER_WARNING", __VA_ARGS__);

#define DEVELOPER_WARNING_IF(condition, ...) \
    if (condition) {                         \
        DEVELOPER_WARNING(__VA_ARGS__)       \
    }
