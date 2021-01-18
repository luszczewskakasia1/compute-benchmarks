#pragma once

#include <cstddef>

class WorkloadSynchronization {
  public:
    WorkloadSynchronization(size_t iterationsCount);

    void synchronize();
    bool validate();

  private:
    size_t expectedSynchronizationCount;
    size_t synchronizationCount = 0;
};
