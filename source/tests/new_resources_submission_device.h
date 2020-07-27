#pragma once

#include "framework/test_case.h"

#include <sstream>

namespace UllsTest {

struct NewResourcesSubmissionDeviceArguments : TestCaseArguments {
    size_t size;

    std::string getHelp() override {
        return "\t\t--size=X\n";
    }

    std::string getCurrentConfig() override {
        std::ostringstream result;
        result << "size=" << size << "";
        return result.str();
    }

    bool parseArgument(const std::string &key, const std::string &value) override {
        if (key == "--size") {
            size = std::atoi(value.c_str());
        }
        return true;
    }

    bool validateArguments() override {
        return size != 0;
    }
};

class NewResourcesSubmissionDevice : public TestCase<NewResourcesSubmissionDeviceArguments> {
  public:
    using TestCase<NewResourcesSubmissionDeviceArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues kernel with a new device resource to measure resource preparation time.";
    };

    std::string getTestCaseName() override {
        return "NewResourcesSubmissionDevice";
    }
};

} // namespace UllsTest
