#pragma once

#include "framework/test_case/test_result.h"

#include <sstream>
#include <string>

class Process {
  public:
    Process(const std::string &exeName);
    Process(Process &&other);
    Process &operator=(Process &&other);
    ~Process();
    void addArgument(const std::string &key, const std::string &value);

    // OS-specific methods
    void run();
    void waitForFinish();
    TestResult getResult();
    const std::string &getStdout();

  private:
    void freeOsSpecificData();

    std::string exeName;
    std::ostringstream commandLine;
    void *osSpecificData = nullptr;
};
