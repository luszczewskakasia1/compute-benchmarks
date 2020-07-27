#include "statistics.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>

Statistics::Statistics(int maxSamplesCount)
    : maxSamplesCount(maxSamplesCount),
      samples(std::make_unique<Value[]>(maxSamplesCount)) {
}

void Statistics::pushValue(Value value) {
    assert(samplesCount <= maxSamplesCount);
    samples[samplesCount++] = value;
}

bool Statistics::isEmpty() {
    return samplesCount == 0;
}

bool Statistics::isFull() {
    return samplesCount == maxSamplesCount;
}

Statistics::Value Statistics::min() {
    return *std::min_element(samples.get(), samples.get() + samplesCount);
}

Statistics::Value Statistics::max() {
    return *std::max_element(samples.get(), samples.get() + samplesCount);
}

Statistics::Value Statistics::mean() {
    return std::accumulate(samples.get(), samples.get() + samplesCount, Value{0}) / samplesCount;
}

Statistics::Value Statistics::median() {
    std::sort(samples.get(), samples.get() + samplesCount);
    if (samplesCount % 2 == 0) {
        const auto left = samples[samplesCount / 2 - 1];
        const auto right = samples[samplesCount / 2];
        return (left + right) / 2;
    } else {
        const auto middle = samples[samplesCount / 2];
        return middle;
    }
}

double Statistics::standardDeviation() {
    const Value mean = this->mean();
    Value diffSum = 0;
    for (int i = 0; i < samplesCount; i++) {
        const auto difference = samples[i] - mean;
        diffSum = difference * difference;
    }
    double stdDev = static_cast<double>(diffSum);
    stdDev /= samplesCount;
    stdDev = std::sqrt(static_cast<double>(stdDev));
    stdDev /= mean;
    return stdDev;
}

constexpr static int columnWidths[] = {52, 15, 15, 15, 15, 15};
constexpr static const char *columnLabels[] = {"TestCase", "Mean [ns]", "Median [ns]", "StdDev", "Min [ns]", "Max [ns]"};

void Statistics::printStatisticsHeader(Configuration::PrintType printType) {
    const int columnCount = sizeof(columnWidths) / sizeof(columnWidths[0]);
    switch (printType) {
    case Configuration::PrintType::Verbose:
    case Configuration::PrintType::Default: {
        for (int column = 0; column < columnCount; column++) {
            std::cout << std::setw(columnWidths[column]) << columnLabels[column];
        }
        std::cout << std::endl;
        break;
    }
    case Configuration::PrintType::Csv:
        for (int column = 0; column < columnCount; column++) {
            std::cout << columnLabels[column];
            if (column != columnCount - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
        break;
    default:
        std::cerr << "ERROR: unknown print type selected\n";
        abort();
    }
}

void Statistics::printStatistics(const std::string &testCaseName, Configuration::PrintType printType) {
    switch (printType) {
    case Configuration::PrintType::Verbose:
    case Configuration::PrintType::Default: {
        int column = 0;
        std::cout << std::setw(columnWidths[column++]) << testCaseName;
        std::cout << std::setw(columnWidths[column++]) << mean();
        std::cout << std::setw(columnWidths[column++]) << median();
        std::cout << std::setw(columnWidths[column++] - 1) << std::fixed << std::setprecision(2) << 100 * standardDeviation() << "%";
        std::cout << std::setw(columnWidths[column++]) << min();
        std::cout << std::setw(columnWidths[column++]) << max();
        std::cout << std::endl;
        break;
    }
    case Configuration::PrintType::Csv: {
        std::cout << testCaseName << ",";
        std::cout << mean() << ",";
        std::cout << median() << ",";
        std::cout << std::fixed << std::setprecision(2) << 100 * standardDeviation() << "%,";
        std::cout << min() << ",";
        std::cout << max();
        std::cout << std::endl;
        break;
    }
    default:
        std::cerr << "ERROR: unknown print type selected\n";
        abort();
    }

    if (printType == Configuration::PrintType::Verbose) {
        for (int i = 0; i < samplesCount; i++) {
            std::cout << samples[i];
            if (i != samplesCount - 1) {
                std::cout << " ";
            } else {
                std::cout << "\n\n";
            }
        }
    }
}
