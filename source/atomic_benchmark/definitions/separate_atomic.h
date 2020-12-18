#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/enum/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/enum/test_case_argument_data_type.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/utility/common_help_message.h"
#include "framework/utility/memory_constants.h"

struct SeparateAtomicsArguments : TestCaseArguments {
    DataTypeTestCaseArgument dataType;
    AtomicOperationTestCaseArgument atomicOperation;
    PositiveIntegerTestCaseArgument atomicsPerCacheline;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    SeparateAtomicsArguments()
        : dataType(*this, "type", CommonHelpMessage::atomicDataType()),
          atomicOperation(*this, "op", "Atomic operation to perform"),
          atomicsPerCacheline(*this, "atomicsPerCacheline", "Number of used addresses occupying a single cacheline (this causes operations to be serialized)"),
          workgroupCount(*this, "wgc", "Work group count"),
          workgroupSize(*this, "wgs", "Work group size") {}

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

struct SeparateAtomics : TestCase<SeparateAtomicsArguments> {
    using TestCase<SeparateAtomicsArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "SeparateAtomics";
    }

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on different addresses";
    }
};
