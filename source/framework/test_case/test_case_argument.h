#pragma once

#include "framework/error.h"
#include "framework/string_utils.h"
#include "framework/transfer_direction.h"

#include <sstream>
#include <string>

struct TestCaseArgument {
    TestCaseArgument(const std::string &key) : TestCaseArgument(key, "") {}
    TestCaseArgument(const std::string &key, const std::string &extraHelp)
        : key(key), extraHelp(extraHelp) {}

    std::string getHelp() const {
        std::ostringstream result;
        result << "--" << key << "=X " << extraHelp;
        return result.str();
    }

    virtual std::string toString() const {
        std::ostringstream result;
        result << key << "=" << toStringValue();
        return result.str();
    }

    void parse(const std::string &key, const std::string &value) {
        if (key == this->key) {
            parseImpl(value);
        }
    }

    virtual bool validate() const {
        return true;
    }

  protected:
    virtual void parseImpl(const std::string &value) = 0;
    virtual std::string toStringValue() const = 0;

  private:
    const std::string key;
    const std::string extraHelp;
};

struct PositiveIntegerTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator size_t() const {
        return value;
    }

    PositiveIntegerTestCaseArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
        return this->value > 0;
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

struct TransferDirectionTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator TransferDirection() const {
        return value;
    }

    TransferDirectionTestCaseArgument &operator=(TransferDirection value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
        return this->value != TransferDirection::Unknown;
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case TransferDirection::HH:
            return "HH";
        case TransferDirection::HD:
            return "HD";
        case TransferDirection::DH:
            return "DH";
        case TransferDirection::DD:
            return "DD";
        default:
            ERROR("Unknown transfer direction");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "hh") {
            this->value = TransferDirection::HH;
        } else if (valueLower == "hd") {
            this->value = TransferDirection::HD;
        } else if (valueLower == "dh") {
            this->value = TransferDirection::DH;
        } else if (valueLower == "dd") {
            this->value = TransferDirection::DD;
        } else {
            this->value = TransferDirection::Unknown;
        }
    }

    TransferDirection value = TransferDirection::Unknown;
};

struct MemoryPlacementTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator MemoryPlacement() const {
        return value;
    }

    MemoryPlacementTestCaseArgument &operator=(MemoryPlacement value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
        return this->value != MemoryPlacement::Unknown;
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case MemoryPlacement::Host:
            return "host";
        case MemoryPlacement::Device:
            return "device";
        default:
            ERROR("Unknown memory placement");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "host") {
            this->value = MemoryPlacement::Host;
        } else if (valueLower == "device") {
            this->value = MemoryPlacement::Device;
        } else {
            this->value = MemoryPlacement::Unknown;
        }
    }

    MemoryPlacement value = MemoryPlacement::Unknown;
};

struct BooleanTestCaseArgument : TestCaseArgument {
    using TestCaseArgument::TestCaseArgument;

    operator bool() const {
        return value != 0;
    }

    BooleanTestCaseArgument &operator=(bool value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
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
