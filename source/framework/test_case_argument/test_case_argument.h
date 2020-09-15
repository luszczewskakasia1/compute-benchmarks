#pragma once

#include "framework/utility/error.h"

#include <sstream>
#include <string>

struct TestCaseArguments;

struct TestCaseArgument {
    TestCaseArgument(TestCaseArguments &parent, const std::string &key) : TestCaseArgument(parent, key, "") {}
    TestCaseArgument(TestCaseArguments &parent, const std::string &key, const std::string &extraHelp);

    std::string getHelp() const {
        std::ostringstream result;
        result << "--" << key << "=X " << extraHelp;
        return result.str();
    }

    virtual std::string toString() const {
        std::ostringstream result;
        result << key << "=" << toStringValue();
        return result.str();
    }

    void parse(const std::string &key, const std::string &value) {
        if (key == this->key) {
            parseImpl(value);
        }
    }

    virtual bool validate() const {
        return true;
    }

  protected:
    virtual void parseImpl(const std::string &value) = 0;
    virtual std::string toStringValue() const = 0;

  private:
    const std::string key;
    const std::string extraHelp;
};
