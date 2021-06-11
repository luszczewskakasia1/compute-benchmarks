#pragma once

#include "framework/argument/abstract/argument.h"
#include "framework/utility/string_utils.h"

#include <vector>

struct StringArgument : Argument {
    using Argument::Argument;

    operator const std::string &() const {
        return value;
    }

    StringArgument &operator=(const std::string &value) {
        this->value = value;
        this->isValid = true;
        markAsParsed();
        return *this;
    }

    bool validate() const override {
        return isValid;
    }

  protected:
    std::string toStringValue() const override {
        return value;
    }

    void parseImpl(const std::string &value) override {
        this->value = value;
    }

    std::string value = {};
    bool isValid = false;
};
