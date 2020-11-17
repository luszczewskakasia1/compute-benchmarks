#pragma once

#include <sstream>
#include <string>

enum class AtomicOperation {
    Unknown,
    Add,
    Sub,
    Xchg,
    CmpXchg,
    Inc,
    Dec,
    Min,
    Max,
    And,
    Or,
    Xor,
};
