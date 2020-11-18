#pragma once

#include "framework/utility/error.h"

enum class DataType {
    Unknown,
    Int32,
    Float,
};

namespace DataTypeHelper {
constexpr inline size_t getSize(DataType dataType) {
    switch (dataType) {
    case DataType::Int32:
        return 4u;
    case DataType::Float:
        return sizeof(float);
    case DataType::Unknown:
        ERROR("Unkown data type")
    }
}
} // namespace DataTypeHelper
