#pragma once

#include "framework/test_case_argument/abstract/test_case_argument.h"

struct IntegerTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator size_t() const {
        return value;
    }

    IntegerTestCaseArgument &operator=(size_t value) {
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

struct PositiveIntegerTestCaseArgument : IntegerTestCaseArgument {
    using IntegerTestCaseArgument::IntegerTestCaseArgument;

    operator size_t() const {
        return value;
    }

    PositiveIntegerTestCaseArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return this->value > 0;
    }
};

struct NonNegativeIntegerTestCaseArgument : IntegerTestCaseArgument {
    using IntegerTestCaseArgument::IntegerTestCaseArgument;

    operator size_t() const {
        return value;
    }

    NonNegativeIntegerTestCaseArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return this->value >= 0;
    }
};

struct ByteSizeTestCaseArgument : PositiveIntegerTestCaseArgument {
    using PositiveIntegerTestCaseArgument::PositiveIntegerTestCaseArgument;

    ByteSizeTestCaseArgument &operator=(size_t value) {
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

struct BooleanTestCaseArgument : TestCaseArgument {
    BooleanTestCaseArgument(TestCaseArgumentsBase &parent, const std::string &key, const std::string &extraHelp)
        : TestCaseArgument(parent, key, extraHelp + " (0 or 1)") {}
    BooleanTestCaseArgument(TestCaseArgumentsBase &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(0 or 1)") {}

    operator bool() const {
        return value != 0;
    }

    BooleanTestCaseArgument &operator=(bool value) {
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
