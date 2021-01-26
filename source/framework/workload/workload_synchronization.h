#pragma once

#include <cstddef>

class WorkloadSynchronization {
  public:
    WorkloadSynchronization(size_t iterationsCount, bool synchronizationEnabled);

    void synchronize();
    bool validate();

    void executeRemainingSynchronizations();

  private:
    size_t synchronizationCount = 0;
    size_t expectedSynchronizationCount;
    bool synchronizationEnabled;
};
