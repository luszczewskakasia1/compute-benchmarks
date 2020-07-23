#pragma once

#include "framework/test_case.h"

#include <sstream>

namespace UllsTest {

struct NewResourcesSubmissionArguments : TestCaseArguments {
    size_t sizeMB;

    std::string getHelp() override {
        return "\t--sizeMB=X\n";
    }

    std::string getCurrentConfig() override {
        std::ostringstream result;
        result << "sizeMB=" << sizeMB << "";
        return result.str();
    }

    bool parseArgument(const std::string &key, const std::string &value) override {
        if (key == "--sizeMB") {
            sizeMB = std::atoi(value.c_str());
        }
        return true;
    }

    bool validateArguments() override {
        return sizeMB != 0;
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
