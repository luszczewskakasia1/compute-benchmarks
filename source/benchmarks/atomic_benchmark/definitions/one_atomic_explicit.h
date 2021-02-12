#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_memory_order.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_scope.h"
#include "framework/test_case_argument/enum/test_case_argument_data_type.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct OneAtomicExplicitArguments : Arguments {
    DataTypeArgument dataType;
    AtomicOperationArgument atomicOperation;
    AtomicScopeArgument scope;
    AtomicMemoryOrderArgument memoryOrder;
    PositiveIntegerArgument workgroupCount;
    PositiveIntegerArgument workgroupSize;

    OneAtomicExplicitArguments()
        : dataType(*this, "type", CommonHelpMessage::atomicDataType()),
          atomicOperation(*this, "op", "Atomic operation to perform"),
          scope(*this, "scope", "Memory scope of an atomic operation"),
          memoryOrder(*this, "order", "Memory order of an atomic operation"),
          workgroupCount(*this, "wgc", "Work group count"),
          workgroupSize(*this, "wgs", "Work group size") {}
};

struct OneAtomicExplicit : TestCase<OneAtomicExplicitArguments> {
    using TestCase<OneAtomicExplicitArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "OneAtomicExplicit";
    }

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on a single address using OpenCL 2.0 Atomics with explicit memory order and scope";
    }
};
