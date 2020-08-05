#pragma once

#include "framework/test_case/test_case.h"
#include "framework/transfer_direction.h"

#include <sstream>

struct UsmCopyArguments : TestCaseArguments {
    TransferDirectionTestCaseArgument transferDirection;
    PositiveIntegerTestCaseArgument size;

    UsmCopyArguments()
        : transferDirection("transfer", "(HH, HD, DH or DD)"),
          size("size") {
        arguments.push_back(&transferDirection);
        arguments.push_back(&size);
    }
};

class UsmCopy : public TestCase<UsmCopyArguments> {
  public:
    using TestCase<UsmCopyArguments>::TestCase;

    std::string getHelp() const override {
        return "allocated two unified shared memory buffers and measures copy bandwidth between them.";
    }

    std::string getTestCaseName() const override {
        return "UsmCopy";
    }
};
