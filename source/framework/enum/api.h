#pragma once

#include "framework/utility/error.h"

#include <string>

enum class Api {
    // Default, invalid value
    Unknown = 0,

    // Supported APIs
    OpenCL,
    L0,

    // Special values
    COUNT,
    FIRST = OpenCL,
    LAST = L0,
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
        FATAL_ERROR("Unknown API");
    }
}
} // namespace std

inline std::string getUserFriendlyApiName(Api api) {
    switch (api) {
    case Api::OpenCL:
        return "OpenCL";
    case Api::L0:
        return "LevelZero";
    default:
        FATAL_ERROR("Unknown API");
    }
}

inline Api parseApi(const std::string &value) {
    if (value == "ocl") {
        return Api::OpenCL;
    } else if (value == "l0") {
        return Api::L0;
    } else if (value == "all") {
        return Api::All;
    } else {
        return Api::Unknown;
    }
}

inline bool validateApi(Api api) {
    switch (api) {
    case Api::OpenCL:
    case Api::L0:
        return true;
    default:
        return false;
    }
}
