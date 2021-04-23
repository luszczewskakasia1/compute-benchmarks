#pragma once

#include "framework/utility/error.h"

enum class UsmMemoryPlacement {
    Unknown,
    Host,
    Device,
    Shared,
    NonUsm
};
