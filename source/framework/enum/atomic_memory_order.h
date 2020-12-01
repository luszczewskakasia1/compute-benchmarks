#pragma once

#include <array>

enum class AtomicMemoryOrder {
    Unknown,
    Relaxed,
    Acquire,
    Release,
    AcquireRelease,
    SequentialConsitent,
};

namespace AtomicMemoryOrderHelper {
constexpr inline std::array<AtomicMemoryOrder, 5> allValues = {
    AtomicMemoryOrder::Relaxed,
    AtomicMemoryOrder::Acquire,
    AtomicMemoryOrder::Release,
    AtomicMemoryOrder::AcquireRelease,
    AtomicMemoryOrder::SequentialConsitent,
};
}
