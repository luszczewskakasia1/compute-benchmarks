#pragma once

#include "framework/workload/workload_argument_container.h"

#include <string>
#include <memory>

class WorkloadIo {
  public:
    // Os-specific factory function
    static std::unique_ptr<WorkloadIo> create(const WorkloadArgumentContainer &arguments);

    virtual void writeToConsole(const std::string &message) = 0;
    virtual void writeToMeasurements(const std::string &measurements) = 0;
    virtual void writeSynchronizationChar(char c) = 0;
    virtual char readSynchronizationChar() = 0;
};
