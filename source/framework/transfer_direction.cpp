#include "transfer_direction.h"

#include "framework/error.h"

bool isDeviceMemory(TransferDirection direction, TransferOperand operand) {
    ERROR_IF(direction == TransferDirection::Unknown, "Unknown direction");
    ERROR_IF(operand == TransferOperand::Unknown, "Unknown operand");

    switch (direction) {
    case TransferDirection::HH:
        return false;
    case TransferDirection::HD:
        return operand == TransferOperand::Destination;
    case TransferDirection::DH:
        return operand == TransferOperand::Source;
    case TransferDirection::DD:
        return true;
    }

    ERROR("Unreachable code");
}

bool isDeviceMemory(MemoryPlacement placement) {
    switch (placement) {
    case MemoryPlacement::Device:
        return true;
    case MemoryPlacement::Host:
        return false;
    default:
        ERROR("Unknown placement");
    }
}
