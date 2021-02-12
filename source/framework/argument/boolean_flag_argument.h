#pragma once

#include "framework/argument/abstract/argument.h"

struct BooleanFlagArgument : Argument {
    using Argument::Argument;

    operator bool() const {
        return value;
    }

    BooleanFlagArgument &operator=(bool value) {
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
