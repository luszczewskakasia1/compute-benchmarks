#pragma once

#include "framework/argument/basic_argument.h"
#include "framework/configuration.h"

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
