#pragma once

#include <string>

struct TestCaseInterface {
    virtual ~TestCaseInterface() = default;
    virtual bool runFromCommandLine(int argc, char **argv) = 0;
    virtual std::string getHelp() const = 0;
    virtual std::string getHelpParameters() const = 0;
    virtual std::string getTestCaseName() const = 0;
};
