#pragma once

#include "framework/error.h"
#include "framework/map_flags.h"
#include "framework/string_utils.h"
#include "framework/transfer_direction.h"

#include <sstream>
#include <string>
#include <tuple>

struct TestCaseArguments;

struct TestCaseArgument {
    TestCaseArgument(TestCaseArguments &parent, const std::string &key) : TestCaseArgument(parent, key, "") {}
    TestCaseArgument(TestCaseArguments &parent, const std::string &key, const std::string &extraHelp);

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

    bool validate() const override {
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

struct ByteSizeTestCaseArgument : PositiveIntegerTestCaseArgument {
    using PositiveIntegerTestCaseArgument::PositiveIntegerTestCaseArgument;

    ByteSizeTestCaseArgument &operator=(size_t value) {
        this->value = value;
        return *this;
    }

    std::string toStringValue() const override {
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
        const std::string units[] = {"KB", "MB", "GB", "B", ""};
        const size_t unitMultipliers[] = {1024, 1024 * 1024, 1024 * 1024 * 1024, 1, 1};
        const auto unitCount = sizeof(units) / sizeof(units[0]);
        static_assert(unitCount == sizeof(unitMultipliers) / sizeof(unitMultipliers[0]));

        std::string valueWithoutUnit;
        auto currentUnit = 0u;
        for (; currentUnit < unitCount; currentUnit++) {
            const auto unitPosition = value.rfind(units[currentUnit]);
            const auto expectedUnitPosition = value.length() - units[currentUnit].length();
            if (unitPosition == expectedUnitPosition) {
                valueWithoutUnit = value.substr(0, unitPosition);
                break;
            }
        }

        this->value = std::atoi(valueWithoutUnit.c_str());
        this->value *= unitMultipliers[currentUnit];
    }
};

struct TransferDirectionTestCaseArgument : TestCaseArgument {
    TransferDirectionTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(SysToSys, SysToDev, DevToSys or DevToDev)") {}

    operator TransferDirection() const {
        return value;
    }

    TransferDirectionTestCaseArgument &operator=(TransferDirection value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
        return this->value != TransferDirection::Unknown;
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case TransferDirection::SysToSys:
            return "SysToSys";
        case TransferDirection::SysToDev:
            return "SysToDev";
        case TransferDirection::DevToSys:
            return "DevToSys";
        case TransferDirection::DevToDev:
            return "DevToDev";
        default:
            ERROR("Unknown transfer direction");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "systosys") {
            this->value = TransferDirection::SysToSys;
        } else if (valueLower == "systodev") {
            this->value = TransferDirection::SysToDev;
        } else if (valueLower == "devtosys") {
            this->value = TransferDirection::DevToSys;
        } else if (valueLower == "devtodev") {
            this->value = TransferDirection::DevToDev;
        } else {
            this->value = TransferDirection::Unknown;
        }
    }

    TransferDirection value = TransferDirection::Unknown;
};

struct MemoryPlacementTestCaseArgument : TestCaseArgument {
    MemoryPlacementTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(host or device)") {}

    operator MemoryPlacement() const {
        return value;
    }

    MemoryPlacementTestCaseArgument &operator=(MemoryPlacement value) {
        this->value = value;
        return *this;
    }

    bool validate() const override {
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
    BooleanTestCaseArgument(TestCaseArguments &parent, const std::string &key)
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

struct MapFlagsTestCaseArgument : TestCaseArgument {
    MapFlagsTestCaseArgument(TestCaseArguments &parent, const std::string &key)
        : TestCaseArgument(parent, key, "(read, write or writeInvalidate)") {}

    operator MapFlags() const {
        return value;
    }

    MapFlagsTestCaseArgument &operator=(MapFlags value) {
        this->value = value;
        return *this;
    }

    bool validate() const {
        switch (value) {
        case MapFlags::Read:
        case MapFlags::Write:
        case MapFlags::WriteInvalidate:
            return true;
        default:
            return false;
        }
    }

  protected:
    std::string toStringValue() const override {
        switch (value) {
        case MapFlags::Read:
            return "read";
        case MapFlags::Write:
            return "write";
        case MapFlags::WriteInvalidate:
            return "writeInvalidate";
        default:
            ERROR("Unknown map flag");
        }
    }

    void parseImpl(const std::string &value) override {
        const std::string valueLower = toLower(value);
        if (valueLower == "read") {
            this->value = MapFlags::Read;
        } else if (valueLower == "write") {
            this->value = MapFlags::Write;
        } else if (valueLower == "writeinvalidate") {
            this->value = MapFlags::WriteInvalidate;
        } else {
            this->value = MapFlags::Unknown;
        }
    }

    MapFlags value = MapFlags::Unknown;
};
