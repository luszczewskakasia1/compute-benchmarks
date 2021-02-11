#include "workload_io.h"

#include <iostream>

void WorkloadIo::writeToConsole(const std::string &message) {
    std::cerr << message;
}

void WorkloadIo::writeToMeasurements(uint64_t measurement) {
    std::cout << measurement << ' ';
}

void WorkloadIo::writeSynchronizationChar(char c) {
    std::cout << c;
}

char WorkloadIo::readSynchronizationChar() {
    return std::cin.get();
}
