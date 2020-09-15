#pragma once

#include <exception>
#include <iostream>

#define ERROR(message)                              \
    std::cerr << "ERROR: " << message << std::endl; \
    throw std::exception();

#define ERROR_IF(condition, message) \
    if (condition) {                 \
        ERROR(message)               \
    }

#define ERROR_UNLESS(condition, message) ERROR_IF(!condition, message)
