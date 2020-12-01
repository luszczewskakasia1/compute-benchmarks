#pragma once

#include <array>

enum class AtomicScope {
    Unknown,
    Workgroup,
    Device,
};

namespace AtomicScopeHelper {
constexpr inline std::array<AtomicScope, 2> allValues = {
    AtomicScope::Workgroup,
    AtomicScope::Device,
};
}
