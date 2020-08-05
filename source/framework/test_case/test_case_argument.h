#pragma once

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

    size_t value;
};
