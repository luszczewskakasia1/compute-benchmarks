#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct UsmCopyArguments : TestCaseArguments {
    TransferDirectionTestCaseArgument transferDirection;
    ByteSizeTestCaseArgument size;

    UsmCopyArguments()
        : transferDirection(*this, "transfer"),
          size(*this, "size") {}
};

class UsmCopy : public TestCase<UsmCopyArguments> {
  public:
    using TestCase<UsmCopyArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates two unified shared memory buffers and measures copy bandwidth between them.";
    }

    std::string getTestCaseName() const override {
        return "UsmCopy";
    }
};
