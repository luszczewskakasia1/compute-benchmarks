#pragma once

#include "framework/test_case.h"

#include <sstream>

struct EmptyKernelArguments : TestCaseArguments {
    size_t workgroupCount;
    size_t workgroupSize;

    std::string getHelp() const override {
        return "\t\t--workgroupCount=X\n"
               "\t\t--workgroupSize=X\n";
    }

    std::string getCurrentConfig() const override {
        const auto gws = workgroupCount * workgroupSize;
        const auto lws = workgroupSize;
        std::ostringstream result;
        result << "gws=" << gws << " lws=" << lws << "";
        return result.str();
    }

    bool parseArgument(const std::string &key, const std::string &value) override {
        if (key == "--workgroupCount") {
            workgroupCount = std::atoi(value.c_str());
        }
        if (key == "--workgroupSize") {
            workgroupSize = std::atoi(value.c_str());
        }
        return true;
    }

    bool validateArguments() const override {
        return workgroupCount != 0 && workgroupSize != 0;
    }
};

class EmptyKernel : public TestCase<EmptyKernelArguments> {
  public:
    using TestCase<EmptyKernelArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues empty kernel to measure walker spawn time.";
    }

    std::string getTestCaseName() const override {
        return "EmptyKernel";
    }
};
