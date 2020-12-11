#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_memory_order.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_scope.h"
#include "framework/test_case_argument/enum/test_case_argument_data_type.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/memory_constants.h"

struct SeparateAtomicsExplicitArguments : TestCaseArguments {
    DataTypeTestCaseArgument dataType;
    AtomicOperationTestCaseArgument atomicOperation;
    PositiveIntegerTestCaseArgument atomicsPerCacheline;
    AtomicScopeTestCaseArgument scope;
    AtomicMemoryOrderTestCaseArgument memoryOrder;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    SeparateAtomicsExplicitArguments()
        : dataType(*this, "type", "data type of the atomic. Keep in mind not all operations are supported for floating poins"),
          atomicOperation(*this, "op", "atomic operation to perform"),
          atomicsPerCacheline(*this, "atomicsPerCacheline", "Number of used addresses occupying a single cacheline (this causes operations to be serialized)"),
          scope(*this, "scope", "memory scope of an atomic operation"),
          memoryOrder(*this, "order", "memory order of an atomic operation"),
          workgroupCount(*this, "wgc", "work group count"),
          workgroupSize(*this, "wgs", "work group size") {}

    bool validateArgumentsExtra() const override {
        if (atomicsPerCacheline > workgroupCount * workgroupSize) {
            return false;
        }
        if (atomicsPerCacheline > MemoryConstants::cachelineSize / DataTypeHelper::getSize(dataType)) {
            return false;
        }
        return true;
    }
};

class SeparateAtomicsExplicit : public TestCase<SeparateAtomicsExplicitArguments> {
  public:
    using TestCase<SeparateAtomicsExplicitArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on different addresses";
    }

    std::string getTestCaseName() const override {
        return "SeparateAtomicsExplicit";
    }
};
