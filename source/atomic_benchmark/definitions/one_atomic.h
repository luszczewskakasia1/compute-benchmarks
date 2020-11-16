#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct OneAtomicArguments : TestCaseArguments {
    AtomicOperationTestCaseArgument atomicOperation;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    OneAtomicArguments()
        : workgroupCount(*this, "wgc", "work group count"),
          workgroupSize(*this, "wgs", "work group size"),
          atomicOperation(*this, "op", "atomic operation to perform") {}
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
