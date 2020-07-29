#pragma once

#include "framework/test_case.h"

#include <sstream>

namespace UllsTest {

struct NewResourcesSubmissionHostArguments : TestCaseArguments {
    size_t size;

    std::string getHelp() const override {
        return "\t\t--size=X\n";
    }

    std::string getCurrentConfig() const override {
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

    bool validateArguments() const override {
        return size != 0;
    }
};

class NewResourcesSubmissionHost : public TestCase<NewResourcesSubmissionHostArguments> {
  public:
    using TestCase<NewResourcesSubmissionHostArguments>::TestCase;

    std::string getHelp() const override {
        return "enqueues kernel with a new host resource to measure resource preparation time.";
    };

    std::string getTestCaseName() const override {
        return "NewResourcesSubmissionHost";
    }
};

} // namespace UllsTest
