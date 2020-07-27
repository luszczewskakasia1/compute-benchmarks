#pragma once

#include "framework/test_case.h"

#include <sstream>

namespace UllsTest {

struct NewResourcesSubmissionHostArguments : TestCaseArguments {
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

class NewResourcesSubmissionHost : public TestCase<NewResourcesSubmissionHostArguments> {
  public:
    using TestCase<NewResourcesSubmissionHostArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues kernel with a new host resource to measure resource preparation time.";
    };

    std::string getTestCaseName() override {
        return "NewResourcesSubmissionHost";
    }
};

} // namespace UllsTest
