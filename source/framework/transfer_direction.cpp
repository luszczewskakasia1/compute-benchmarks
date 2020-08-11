#include "transfer_direction.h"

#include "framework/error.h"

bool isDeviceMemory(TransferDirection direction, TransferOperand operand) {
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
