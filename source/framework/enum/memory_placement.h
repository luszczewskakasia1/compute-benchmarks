#pragma once

#include "framework/utility/error.h"

enum class MemoryPlacement {
    Unknown,
    Host,
    Device,
    Shared,
};
