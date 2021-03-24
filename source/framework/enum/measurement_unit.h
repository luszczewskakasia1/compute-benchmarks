#pragma once

#include "framework/utility/error.h"

#include <string>

enum class MeasurementUnit {
    Microseconds,
    GigabytesPerSecond,
};

namespace std {
inline std::string to_string(MeasurementUnit unit) {
    switch (unit) {
    case MeasurementUnit::Microseconds:
        return "us";
    case MeasurementUnit::GigabytesPerSecond:
        return "GB/s";
    default:
        FATAL_ERROR("Unknown measurement unit");
    }
}
} // namespace std
