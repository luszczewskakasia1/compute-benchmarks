#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/enum/test_case_argument_data_type.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"

struct OneAtomicArguments : TestCaseArguments {
    DataTypeTestCaseArgument dataType;
    AtomicOperationTestCaseArgument atomicOperation;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    OneAtomicArguments()
        : dataType(*this, "type", CommonHelpMessage::atomicDataType()),
          atomicOperation(*this, "op", "Atomic operation to perform"),
          workgroupCount(*this, "wgc", "Work group count"),
          workgroupSize(*this, "wgs", "Work group size") {}
};

struct OneAtomic : TestCase<OneAtomicArguments> {
    using TestCase<OneAtomicArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "OneAtomic";
    }

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on a single address";
    }
};
