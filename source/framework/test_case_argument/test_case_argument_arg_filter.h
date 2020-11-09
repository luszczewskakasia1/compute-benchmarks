#pragma once

#include "framework/test_case_argument/abstract/test_case_argument.h"
#include "framework/utility/string_utils.h"

#include <vector>

struct ArgFilterTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator const std::vector<std::string> &() const {
        return get();
    }

    const std::vector<std::string> &get() const {
        return value;
    }

    ArgFilterTestCaseArgument &operator=(const std::vector<std::string> &value) {
        this->value = value;
        this->isValid = true;
        return *this;
    }

    bool validate() const override {
        return isValid;
    }

  protected:
    std::string toStringValue() const override {
        ERROR("ArgFilterTestCaseArgument should not be printed");
    }

    void parseImpl(const std::string &value) override {
        for (const auto &filter : splitString(value)) {
            this->value.push_back(filter);
        }
    }

    std::vector<std::string> value = {};
    bool isValid = false;
};
