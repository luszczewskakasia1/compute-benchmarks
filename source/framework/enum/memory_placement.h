#pragma once

#include "framework/error.h"

enum class MemoryPlacement {
    Unknown,
    Host,
    Device,
};

inline bool isDeviceMemory(MemoryPlacement placement) {
    switch (placement) {
    case MemoryPlacement::Device:
        return true;
    case MemoryPlacement::Host:
        return false;
    }

    ERROR("Unknown placement");
}
