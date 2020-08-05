#pragma once

enum class TransferDirection {
    Unknown,
    HH,
    HD,
    DH,
    DD,
};

enum class TransferOperand {
    Unknown,
    Source,
    Destination
};

bool isDeviceMemory(TransferDirection direction, TransferOperand operand);
