#pragma once

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

enum class MemoryPlacement {
    Unknown,
    Host,
    Device,
};

bool isDeviceMemory(TransferDirection direction, TransferOperand operand);
bool isDeviceMemory(MemoryPlacement placement);
