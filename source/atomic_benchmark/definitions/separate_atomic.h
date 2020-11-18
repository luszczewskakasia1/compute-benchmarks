#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_atomic_operation.h"
#include "framework/test_case_argument/test_case_argument_basic.h"
#include "framework/test_case_argument/test_case_argument_data_type.h"
#include "framework/utility/memory_constants.h"

struct SeparateAtomicsArguments : TestCaseArguments {
    DataTypeTestCaseArgument dataType;
    AtomicOperationTestCaseArgument atomicOperation;
    PositiveIntegerTestCaseArgument atomicsPerCacheline;
    PositiveIntegerTestCaseArgument workgroupCount;
    PositiveIntegerTestCaseArgument workgroupSize;

    SeparateAtomicsArguments()
        : dataType(*this, "type", "data type of the atomic. Keep in mind not all operations are supported for floating poins"),
          atomicOperation(*this, "op", "atomic operation to perform"),
          atomicsPerCacheline(*this, "atomicsPerCacheline", "Number of used addresses occupying a single cacheline (this causes operations to be serialized)"),
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

class SeparateAtomics : public TestCase<SeparateAtomicsArguments> {
  public:
    using TestCase<SeparateAtomicsArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel performing an atomic operation on different addresses";
    }

    std::string getTestCaseName() const override {
        return "SeparateAtomics";
    }
};
