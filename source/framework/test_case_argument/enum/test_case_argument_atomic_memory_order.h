#pragma once

#include "framework/enum/atomic_memory_order.h"
#include "framework/test_case_argument/abstract/test_case_argument_enum.h"

struct AtomicMemoryOrderTestCaseArgument : EnumTestCaseArgument<AtomicMemoryOrderTestCaseArgument, AtomicMemoryOrder> {
    using EnumTestCaseArgument::EnumTestCaseArgument;
    ThisType &operator=(EnumType value) {
        this->value = value;
        return *this;
    }

    const static inline std::string enumName = "atomic memory order";
    const static inline EnumType invalidEnumValue = EnumType::Unknown;
    const static inline EnumType enumValues[5] = {AtomicMemoryOrder::Relaxed,
                                                  AtomicMemoryOrder::Acquire,
                                                  AtomicMemoryOrder::Release,
                                                  AtomicMemoryOrder::AcquireRelease,
                                                  AtomicMemoryOrder::SequentialConsitent};
    const static inline std::string enumValuesNames[5] = {"relaxed",
                                                          "acquire",
                                                          "release",
                                                          "acq_rel",
                                                          "seq_cst"};
};
