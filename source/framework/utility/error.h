#pragma once

#include <exception>
#include <iostream>

template <typename... Args>
inline void printFatalError(const char *label, Args &&... args) {
    static_assert(sizeof...(args) > 0, "A textual message of the FATAL_ERROR is required");
    std::cerr << label << ": ";
    (std::cerr << ... << args);
    std::cerr << std::endl;
}

#define FATAL_ERROR(...)                   \
    printFatalError("ERROR", __VA_ARGS__); \
    throw std::exception();

#define FATAL_ERROR_IF(condition, ...) \
    if (condition) {                   \
        FATAL_ERROR(__VA_ARGS__)       \
    }

#define FATAL_ERROR_UNLESS(condition, message) FATAL_ERROR_IF(!(condition), (message))

#define FATAL_ERROR_IN_DESTRUCTOR(...)     \
    printFatalError("ERROR", __VA_ARGS__); \
    std::abort();
