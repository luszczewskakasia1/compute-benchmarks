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
        ERROR(""); // TODO
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

    virtual bool validate() const {
        return value[0] > 0 &&
               value[1] > 0 &&
               value[2] > 0;
    }
};
