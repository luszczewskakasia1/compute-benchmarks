#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/argument/enum/work_item_id_usage_argument.h"
#include "framework/test_case/test_case.h"
#include "framework/utility/common_help_message.h"

struct BarrierBetweenKernelsArguments : TestCaseArgumentContainer {
    PositiveIntegerArgument bytesToFlush;
    IntegerArgument onlyReads;

    BarrierBetweenKernelsArguments()
        : bytesToFlush(*this, "bytes", "bytes to flush from L3"),
          onlyReads(*this, "onlyReads", "only reads cached in L3") {}
};

struct BarrierBetweenKernels : TestCase<BarrierBetweenKernelsArguments> {

    using TestCase<BarrierBetweenKernelsArguments>::TestCase;

    std::string getTestCaseName() const override {
        return "BarrierBetweenKernels";
    }

    std::string getHelp() const override {
        return "measures time required to run a barrier command between 2 kernels, including potential cache flush commands";
    }
};
