#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct KernelAndCopyArguments : TestCaseArguments {
    BooleanTestCaseArgument twoQueues;
    BooleanTestCaseArgument runKernel;
    BooleanTestCaseArgument runCopy;

    KernelAndCopyArguments()
        : twoQueues(*this, "twoQueues", "enables using separate queues for both operations"),
          runKernel(*this, "runKernel", "enqueue kernel during each iteration"),
          runCopy(*this, "runCopy", "enqueue buffer to buffer copy during each iteration") {}
};

class KernelAndCopy : public TestCase<KernelAndCopyArguments> {
  public:
    using TestCase<KernelAndCopyArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel and copy operation with the ability to perform both tasks on "
               "different command queues.";
    }

    std::string getTestCaseName() const override {
        return "KernelAndCopy";
    }
};
