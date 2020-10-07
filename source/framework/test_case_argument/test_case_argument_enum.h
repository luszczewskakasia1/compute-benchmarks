#pragma once

#include "framework/test_case_argument/test_case_argument.h"
#include "framework/utility/string_utils.h"

template <typename DerivedType, typename _EnumType>
struct EnumTestCaseArgument : TestCaseArgument {
    using EnumType = _EnumType;
    using ThisType = DerivedType;

    EnumTestCaseArgument(TestCaseArgumentsBase &parent, const std::string &key)
        : TestCaseArgument(parent, key, composeHelpMessage()) {
        static_assert(sizeof(DerivedType::enumValues) / sizeof(DerivedType::enumValues[0]) ==
                      sizeof(DerivedType::enumValuesNames) / sizeof(DerivedType::enumValuesNames[0]));
    }

    operator EnumType() const {
        return value;
    }

    bool validate() const {
        const auto valuesCount = sizeof(DerivedType::enumValues) / sizeof(DerivedType::enumValues[0]);
        for (auto valueIndex = 0; valueIndex < valuesCount; valueIndex++) {
            if (this->value == DerivedType::enumValues[valueIndex]) {
                return true;
            }
        }
        return false;
    }

  protected:
    std::string toStringValue() const override {
        const auto valuesCount = sizeof(DerivedType::enumValues) / sizeof(DerivedType::enumValues[0]);
        for (auto valueIndex = 0; valueIndex < valuesCount; valueIndex++) {
            if (this->value == DerivedType::enumValues[valueIndex]) {
                return DerivedType::enumValuesNames[valueIndex];
            }
        }
        ERROR((std::string("Unknown ") + DerivedType::enumName));
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        const auto valuesCount = sizeof(DerivedType::enumValues) / sizeof(DerivedType::enumValues[0]);
        for (auto valueIndex = 0; valueIndex < valuesCount; valueIndex++) {
            if (valueLower == toLower(DerivedType::enumValuesNames[valueIndex])) {
                this->value = DerivedType::enumValues[valueIndex];
                return;
            }
        }
        this->value = DerivedType::invalidEnumValue;
    }

    EnumType value;

  private:
    static std::string composeHelpMessage() {
        std::ostringstream out{};
        out << "(";

        const auto enumValuesNamesCount = sizeof(DerivedType::enumValuesNames) / sizeof(DerivedType::enumValuesNames[0]);
        for (int i = 0; i < enumValuesNamesCount; i++) {
            out << DerivedType::enumValuesNames[i];

            if (i != enumValuesNamesCount - 1) {
                out << " or ";
            }
        }
        out << ")";
        return out.str();
    }
};
