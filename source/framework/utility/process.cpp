#include "process.h"

Process::Process(const std::string &exeName)
    : exeName(exeName) {
    commandLine << exeName << ' ';
}

Process::Process(Process &&other)
    : exeName(std::move(other.exeName)),
      commandLine(std::move(other.commandLine)),
      envVariables(std::move(other.envVariables)),
      osSpecificData(std::move(other.osSpecificData)) {
    other.osSpecificData = nullptr;
}

Process &Process::operator=(Process &&other) {
    exeName = std::move(other.exeName);
    commandLine = std::move(other.commandLine);
    envVariables = std::move(other.envVariables);
    osSpecificData = std::move(other.osSpecificData);
    other.osSpecificData = nullptr;
    return *this;
}

Process::~Process() {
    freeOsSpecificData();
}

void Process::addArgument(const std::string &key, const std::string &value) {
    commandLine << "--" << key << "=" << value << " ";
}

void Process::addEnvVariable(const std::string &key, const std::string &value) {
    envVariables.emplace_back(key, value);
}
