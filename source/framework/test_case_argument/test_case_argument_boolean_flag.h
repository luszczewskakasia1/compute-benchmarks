#pragma once

#include "framework/test_case_argument/abstract/test_case_argument.h"

struct BooleanFlagTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator bool() const {
        return value;
    }

    BooleanFlagTestCaseArgument &operator=(bool value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return isValid;
    }

  protected:
    std::string toStringValue() const override {
        return std::to_string(this->value);
    }

    void parseImpl(const std::string &value) override {
        this->value = true;
        this->isValid = value.empty();
    }

    std::string getHelpEntry(const std::string &key) const override {
        return std::string("--") + key;
    }

    bool value = false;
    bool isValid = true;
};
