#pragma once

#include "framework/configuration.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct CompressionBooleanArgument : BooleanArgument {
    using BooleanArgument::BooleanArgument;

    CompressionBooleanArgument &operator=(bool value) {
        this->value = value;
        return *this;
    }

  protected:
    std::string toStringValue() const override {
        if (Configuration::get().noIntelExtensions) {
            return "?";
        }
        return BooleanArgument::toStringValue();
    }
};
