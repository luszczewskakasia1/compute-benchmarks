#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_data_type.h"

struct OneAtomicArguments : TestCaseArguments {
    DataTypeTestCaseArgument dataType;
    AtomicOperationTestCaseArgument atomicOperation;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    OneAtomicArguments()
        : dataType(*this, "type", "data type of the atomic. Keep in mind not all operations are supported for floating poins"),
          atomicOperation(*this, "op", "atomic operation to perform"),
          workgroupCount(*this, "wgc", "work group count"),
          workgroupSize(*this, "wgs", "work group size") {}
};

class OneAtomic : public TestCase<OneAtomicArguments> {
  public:
    using TestCase<OneAtomicArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on a single address";
    }

    std::string getTestCaseName() const override {
        return "OneAtomic";
    }
};
