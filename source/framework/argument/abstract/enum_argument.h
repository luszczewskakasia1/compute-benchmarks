/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#pragma once

#include "framework/argument/abstract/argument.h"
#include "framework/utility/string_utils.h"

template <typename DerivedType, typename _EnumType>
struct EnumArgument : Argument {
    using EnumType = _EnumType;
    using ThisType = DerivedType;

    EnumArgument(ArgumentContainer &parent, const std::string &key) : EnumArgument(parent, key, "") {}
    EnumArgument(ArgumentContainer &parent, const std::string &key, const std::string &extraHelpPrefix)
        : Argument(parent, key, composeHelpMessage(extraHelpPrefix)) {
        static_assert(sizeof(DerivedType::enumValues) / sizeof(DerivedType::enumValues[0]) ==
                      sizeof(DerivedType::enumValuesNames) / sizeof(DerivedType::enumValuesNames[0]));
    }

    operator EnumType() const {
        return value;
    }

    bool validate() const override {
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
        FATAL_ERROR((std::string("Unknown ") + DerivedType::enumName));
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
    static std::string composeHelpMessage(const std::string &extraHelpPrefix) {
        std::ostringstream out{};

        if (extraHelpPrefix.size() > 0u) {
            out << extraHelpPrefix << " ";
        }

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
