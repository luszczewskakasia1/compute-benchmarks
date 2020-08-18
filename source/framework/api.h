#pragma once

#include "framework/error.h"

#include <string>

enum class Api {
    // Default, invalid value
    Unknown = 0,

    // Supported APIs
    OpenCL,
    L0,

    // Special values
    FIRST = OpenCL,
    LAST = L0,
    COUNT = LAST - FIRST + 1,
    All = 0xffff,
};

namespace std {
inline std::string to_string(Api api) {
    switch (api) {
    case Api::OpenCL:
        return "ocl";
    case Api::L0:
        return "l0";
    default:
        ERROR("Unknown API");
    }
}
} // namespace std

inline bool validateApi(Api api) {
    switch (api) {
    case Api::OpenCL:
    case Api::L0:
        return true;
    default:
        return false;
    }
}
