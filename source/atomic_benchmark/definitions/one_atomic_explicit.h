#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_memory_order.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_scope.h"
#include "framework/test_case_argument/enum/test_case_argument_data_type.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct OneAtomicExplicitArguments : TestCaseArguments {
    DataTypeTestCaseArgument dataType;
    AtomicOperationTestCaseArgument atomicOperation;
    AtomicScopeTestCaseArgument scope;
    AtomicMemoryOrderTestCaseArgument memoryOrder;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    OneAtomicExplicitArguments()
        : dataType(*this, "type", "data type of the atomic. Keep in mind not all operations are supported for floating points"),
          atomicOperation(*this, "op", "atomic operation to perform"),
          scope(*this, "scope", "memory scope of an atomic operation"),
          memoryOrder(*this, "order", "memory order of an atomic operation"),
          workgroupCount(*this, "wgc", "work group count"),
          workgroupSize(*this, "wgs", "work group size") {}
};

class OneAtomicExplicit : public TestCase<OneAtomicExplicitArguments> {
  public:
    using TestCase<OneAtomicExplicitArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on a single address using OpenCL 2.0 Atomics with explicit memory order and scope";
    }

    std::string getTestCaseName() const override {
        return "OneAtomicExplicit";
    }
};
