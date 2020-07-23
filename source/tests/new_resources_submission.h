#pragma once

#include "framework/test_case.h"

#include <sstream>

namespace UllsTest {

struct NewResourcesSubmissionArguments : TestCaseArguments {
    size_t size;

    std::string getHelp() override {
        return "\t--size=X\n";
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

class NewResourcesSubmission : public TestCase<NewResourcesSubmissionArguments> {
  public:
    using TestCase<NewResourcesSubmissionArguments>::TestCase;

    std::string getHelp() override {
        return "enqueues kernel with a new resource to measure resource preparation time.";
    };

    std::string getTestCaseName() override {
        return "NewResourcesSubmission";
    }
};

} // namespace UllsTest
