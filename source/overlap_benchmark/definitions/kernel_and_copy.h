#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct KernelAndCopyArguments : TestCaseArguments {
    BooleanTestCaseArgument twoQueues;
    BooleanTestCaseArgument runKernel;
    BooleanTestCaseArgument runCopy;
    BooleanTestCaseArgument useCopyQueue;

    KernelAndCopyArguments()
        : twoQueues(*this, "twoQueues", "enables using separate queues for both operations. Must be used with runCopy and runKernel"),
          runKernel(*this, "runKernel", "enqueue kernel during each iteration"),
          runCopy(*this, "runCopy", "enqueue buffer to buffer copy during each iteration"),
          useCopyQueue(*this, "useCopyQueue", "use a specialized copy queue for the copy operation. Must be used with runCopy") {}

    bool validateArgumentsExtra() const override {
        if (!runKernel && !runCopy) {
            return false;
        }
        if ((!runCopy || !runKernel) && twoQueues) {
            return false;
        }
        if (!runCopy && useCopyQueue) {
            return false;
        }
        if (runCopy && runKernel && !twoQueues && useCopyQueue) {
            return false;
        }
        return true;
    }
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
