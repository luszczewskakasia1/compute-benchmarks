#pragma once

#include <exception>
#include <iostream>

#define FATAL_ERROR(message)                          \
    std::cerr << "ERROR: " << (message) << std::endl; \
    throw std::exception();

#define FATAL_ERROR_IF(condition, message) \
    if (condition) {                       \
        FATAL_ERROR(message)               \
    }

#define FATAL_ERROR_UNLESS(condition, message) FATAL_ERROR_IF(!(condition), (message))
