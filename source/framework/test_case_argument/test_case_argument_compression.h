#pragma once

#include "framework/configuration.h"
#include "framework/test_case_argument/test_case_argument_basic.h"

struct CompressionBooleanTestCaseArgument : BooleanTestCaseArgument {
    using BooleanTestCaseArgument::BooleanTestCaseArgument;

    CompressionBooleanTestCaseArgument &operator=(bool value) {
        this->value = value;
        return *this;
    }

  protected:
    std::string toStringValue() const override {
        if (::configuration.noIntelExtensions) {
            return "?";
        }
        return BooleanTestCaseArgument::toStringValue();
    }
};
