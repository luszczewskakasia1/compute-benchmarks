#include "workload_synchronization.h"

#include "framework/utility/error.h"
#include "framework/utility/process_synchronization_helper.h"

#include <iostream>

WorkloadSynchronization::WorkloadSynchronization(size_t iterationsCount, bool synchronizationEnabled)
    : expectedSynchronizationCount(iterationsCount),
      synchronizationEnabled(synchronizationEnabled) {
}

void WorkloadSynchronization::synchronize() {
    FATAL_ERROR_IF(synchronizationCount == expectedSynchronizationCount, "Too many iterations signalled during workload synchronization");
    synchronizationCount++;

    if (!synchronizationEnabled) {
        return;
    }

    // Signal that we're ready
    std::cout << ProcessSynchronizationHelper::synchronizationChar;

    // Wait for signal from master
    char character = {};
    do {
        character = std::cin.get();
    } while (character == '\n' || character == '\r');

    FATAL_ERROR_IF(character != ProcessSynchronizationHelper::synchronizationChar,
                   std::string("Invalid synchronization char. Expected '") + ProcessSynchronizationHelper::synchronizationChar + "'");
}

bool WorkloadSynchronization::validate() {
    return synchronizationCount == expectedSynchronizationCount;
}

void WorkloadSynchronization::executeRemainingSynchronizations() {
    while (synchronizationCount < expectedSynchronizationCount) {
        synchronize();
    }
}
