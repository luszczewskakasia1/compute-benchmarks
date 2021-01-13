#include "process.h"
#include <iostream>
Process::Process(const std::string &exeName)
    : exeName(exeName) {
    arguments.emplace_back(exeName, std::string{});
}

Process::Process(Process &&other)
    : exeName(std::move(other.exeName)),
      arguments(std::move(other.arguments)),
      envVariables(std::move(other.envVariables)),
      osSpecificData(std::move(other.osSpecificData)) {
    other.osSpecificData = nullptr;
}

Process &Process::operator=(Process &&other) {
    exeName = std::move(other.exeName);
    arguments = std::move(other.arguments);
    envVariables = std::move(other.envVariables);
    osSpecificData = std::move(other.osSpecificData);
    other.osSpecificData = nullptr;
    return *this;
}

Process::~Process() {
    freeOsSpecificData();
}

void Process::addArgument(const std::string &key, const std::string &value) {
    arguments.emplace_back(std::string("--") + key, value);
}

void Process::addEnvVariable(const std::string &key, const std::string &value) {
    envVariables.emplace_back(key, value);
}
