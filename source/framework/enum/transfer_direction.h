#pragma once

#include "framework/error.h"

enum class TransferDirection {
    Unknown,
    SysToSys,
    SysToDev,
    DevToSys,
    DevToDev,
};

enum class TransferOperand {
    Unknown,
    Source,
    Destination
};

inline bool isDeviceMemory(TransferDirection direction, TransferOperand operand) {
    ERROR_IF(direction == TransferDirection::Unknown, "Unknown direction");
    ERROR_IF(operand == TransferOperand::Unknown, "Unknown operand");

    switch (direction) {
    case TransferDirection::SysToSys:
        return false;
    case TransferDirection::SysToDev:
        return operand == TransferOperand::Destination;
    case TransferDirection::DevToSys:
        return operand == TransferOperand::Source;
    case TransferDirection::DevToDev:
        return true;
    }

    ERROR("Unreachable code");
}
