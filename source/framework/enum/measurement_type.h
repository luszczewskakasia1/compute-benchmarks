#pragma once

#include "framework/utility/error.h"

#include <string>

enum class MeasurementType {
    Unknown,
    Cpu,
    Gpu,
};

namespace std {
inline std::string to_string(MeasurementType unit) {
    switch (unit) {
    case MeasurementType::Cpu:
        return "[CPU]";
    case MeasurementType::Gpu:
        return "[GPU]";
    default:
        return "";
    }
}
} // namespace std
