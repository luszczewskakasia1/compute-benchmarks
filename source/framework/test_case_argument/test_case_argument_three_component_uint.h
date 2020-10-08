#pragma once

#include "framework/test_case_argument/test_case_argument.h"

#include <tuple>

struct ThreeComponentUintTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;
    using TupleType = std::tuple<size_t, size_t, size_t>;

    operator const size_t *() const {
        return value;
    }

    ThreeComponentUintTestCaseArgument &operator=(TupleType value) {
        assign(value);
        return *this;
    }

  protected:
    void assign(TupleType value) {
        this->value[0] = std::get<0>(value);
        this->value[1] = std::get<1>(value);
        this->value[2] = std::get<2>(value);
    }

    std::string toStringValue() const override {
        std::ostringstream result{};
        result << value[0] << ":"
               << value[1] << ":"
               << value[2];
        return result.str();
    }

    void parseImpl(const std::string &value) override {
        const auto colonPos1 = value.find(":");
        const auto colonPos2 = value.find(":", colonPos1 + 1);
        const auto colonPos3 = value.find(":", colonPos2 + 1);
        ERROR_IF(colonPos1 == std::string::npos, "Too few colons specified for a 3-component vector");
        ERROR_IF(colonPos2 == std::string::npos, "Too few colons specified for a 3-component vector");
        ERROR_IF(colonPos3 != std::string::npos, "Too many colons specified for a 3-component vector");

        const std::string componentsString[3] = {
            value.substr(0, colonPos1),
            value.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1),
            value.substr(colonPos2 + 1),
        };

        this->value[0] = std::atoi(componentsString[0].c_str());
        this->value[1] = std::atoi(componentsString[1].c_str());
        this->value[2] = std::atoi(componentsString[2].c_str());
    }

    size_t value[3] = {0, 0, 0};
};

struct ThreeComponentOffsetTestCaseArgument : ThreeComponentUintTestCaseArgument {
    using ThreeComponentUintTestCaseArgument::ThreeComponentUintTestCaseArgument;

    ThreeComponentOffsetTestCaseArgument &operator=(TupleType value) {
        assign(value);
        return *this;
    }
};

struct ThreeComponentSizeTestCaseArgument : ThreeComponentUintTestCaseArgument {
    using ThreeComponentUintTestCaseArgument::ThreeComponentUintTestCaseArgument;

    ThreeComponentSizeTestCaseArgument &operator=(TupleType value) {
        assign(value);
        return *this;
    }

    bool validate() const override {
        return value[0] > 0 &&
               value[1] > 0 &&
               value[2] > 0;
    }
};
