#pragma once

#include "framework/test_case/test_result.h"

#include <string>
#include <vector>

class Process {
  public:
    Process(const std::string &exeName);
    Process(Process &&other);
    Process &operator=(Process &&other);
    ~Process();
    void addArgument(const std::string &key, const std::string &value);
    void addEnvVariable(const std::string &key, const std::string &value);
    std::vector<uint64_t> getMeasurements(size_t expectedCount);

    // OS-specific methods
    void run();
    void waitForFinish();
    TestResult getResult();
    const std::string &getStdout();
    void synchronizationSignal();
    void synchronizationWait();

  private:
    void freeOsSpecificData();

    std::string exeName;
    std::vector<std::pair<std::string, std::string>> arguments;
    std::vector<std::pair<std::string, std::string>> envVariables;
    void *osSpecificData = nullptr;
};
