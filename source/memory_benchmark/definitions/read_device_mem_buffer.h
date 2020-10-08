#pragma once

#include "framework/test_case/test_case.h"
#include "framework/test_case_argument/test_case_argument_compression.h"

#include <sstream>

constexpr size_t kiloByte = 1024u;
constexpr size_t megaByte = 1024u * kiloByte;
constexpr size_t gigaByte = 1024u * megaByte;

struct ReadDeviceMemBufferArguments : TestCaseArguments {
    ByteSizeTestCaseArgument size;
    BooleanTestCaseArgument compressed;
    //ByteSizeTestCaseArgument numWaves;

    ReadDeviceMemBufferArguments()
        : size(*this, "size"),
          compressed(*this, "compressed")
          //,numWaves(*this, "waves") 
    {}
};

class ReadDeviceMemBuffer : public TestCase<ReadDeviceMemBufferArguments> {
  public:
    using TestCase<ReadDeviceMemBufferArguments>::TestCase;

    std::string getHelp() const override {
        return "allocates two OpenCL buffers and measures source buffer read bandwidth, source buffer resides in device memory.";
    }

    std::string getTestCaseName() const override {
        return "ReadDeviceMemBuffer";
    }
};
