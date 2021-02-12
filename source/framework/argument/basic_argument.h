#pragma once

#include "framework/argument/abstract/argument.h"
#include "framework/utility/string_utils.h"

struct IntegerArgument : Argument {
    using Argument::Argument;

    operator size_t() const {
        return value;
    }

    IntegerArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    size_t getSizeOf() const {
        return sizeof(value);
    }

    const size_t *getAddressOf() const {
        return &value;
    }

  protected:
    std::string toStringValue() const override {
        return std::to_string(this->value);
    }

    void parseImpl(const std::string &value) override {
        this->value = std::atoi(value.c_str());
    }

    size_t value = 0u;
};

struct PositiveIntegerArgument : IntegerArgument {
    using IntegerArgument::IntegerArgument;

    operator size_t() const {
        return value;
    }

    PositiveIntegerArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return this->value > 0;
    }
};

struct NonNegativeIntegerArgument : IntegerArgument {
    using IntegerArgument::IntegerArgument;

    operator size_t() const {
        return value;
    }

    NonNegativeIntegerArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return this->value >= 0;
    }
};

struct ByteSizeArgument : PositiveIntegerArgument {
    using PositiveIntegerArgument::PositiveIntegerArgument;

    ByteSizeArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    std::string toStringValue() const override {
        if (this->value == 0) {
            return "0";
        }

        const std::string units[] = {"", "KB", "MB", "GB"};
        const auto unitCount = sizeof(units) / sizeof(units[0]);

        auto currentUnit = 0u;
        auto currentValue = this->value;
        for (; currentUnit < unitCount; currentUnit++) {
            if (currentValue % 1024 != 0) {
                break;
            }
            currentValue /= 1024;
        }

        return std::to_string(currentValue) + units[currentUnit];
    }

    void parseImpl(const std::string &value) override {
        const std::string units[] = {"kb", "mb", "gb", "b", ""};
        const size_t unitMultipliers[] = {1024, 1024 * 1024, 1024 * 1024 * 1024, 1, 1};
        const auto unitCount = sizeof(units) / sizeof(units[0]);
        static_assert(unitCount == sizeof(unitMultipliers) / sizeof(unitMultipliers[0]));

        const std::string valueLower = toLower(value);
        std::string valueWithoutUnit{};
        auto currentUnit = 0u;
        for (; currentUnit < unitCount; currentUnit++) {
            if (valueLower.length() < units[currentUnit].length()) {
                continue;
            }

            const auto unitPosition = valueLower.rfind(units[currentUnit]);
            const auto expectedUnitPosition = valueLower.length() - units[currentUnit].length();
            if (unitPosition == expectedUnitPosition) {
                valueWithoutUnit = valueLower.substr(0, unitPosition);
                break;
            }
        }

        this->value = std::atoi(valueWithoutUnit.c_str());
        this->value *= unitMultipliers[currentUnit];
    }
};

struct BooleanArgument : Argument {
    BooleanArgument(ArgumentContainer &parent, const std::string &key, const std::string &extraHelp)
        : Argument(parent, key, extraHelp + " (0 or 1)") {}
    BooleanArgument(ArgumentContainer &parent, const std::string &key)
        : Argument(parent, key, "(0 or 1)") {}

    operator bool() const {
        return value != 0;
    }

    BooleanArgument &operator=(bool value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return value == 0 || value == 1;
    }

  protected:
    std::string toStringValue() const override {
        return std::to_string(this->value);
    }

    void parseImpl(const std::string &value) override {
        this->value = std::atoi(value.c_str());
    }

    int value = -1;
};
