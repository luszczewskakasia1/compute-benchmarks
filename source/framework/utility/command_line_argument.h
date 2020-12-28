#pragma once

#include <string>
#include <vector>

class CommandLineArgument {
  public:
    CommandLineArgument(const char *token);
    CommandLineArgument(const CommandLineArgument &) = delete;
    CommandLineArgument &operator=(const CommandLineArgument &) = delete;
    CommandLineArgument(CommandLineArgument &&) = default;
    CommandLineArgument &operator=(CommandLineArgument &&) = default;

    static bool parseArguments(int argc, char **argv, std::vector<CommandLineArgument> &outArguments, std::string &outErrorMessage);

    void markAsProcessed();

    const std::string &getKey() const { return key; }
    const std::string &getValue() const { return value; }
    bool isKeyEqualTo(const std::string &key) const;
    bool isValid() const { return valid; }
    bool isProcessed() const { return processed; }

  private:
    std::string key = "";
    std::string value = "";
    bool valid = true;
    bool processed = false;
};

using CommandLineArguments = std::vector<CommandLineArgument>;
