#include "statistics.h"

#include "framework/benchmark_info.h"
#include "framework/utility/error.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <type_traits>

Statistics::Statistics(size_t maxSamplesCount, Configuration::PrintType printType)
    : maxSamplesCount(maxSamplesCount),
      printType(printType),
      samples(std::make_unique<Value[]>(maxSamplesCount)) {
}

void Statistics::pushValue(Clock::duration time) {
    static_assert(std::is_floating_point_v<Value>, "Need floating point type for the above cast to work properly");
    const Value timeSeconds = std::chrono::duration_cast<std::chrono::duration<Value>>(time).count();

    switch (getMeasurementUnit()) {
    case MeasurementUnit::Microseconds: {
        const Value timeMicroseconds = timeSeconds * 1e6;
        this->pushValue(timeMicroseconds);
        break;
    }
    case MeasurementUnit::GigabytesPerSecond:
        ERROR("Buffer size needs to be passed in bandwidth mode");
    default:
        ERROR("Unknown measurement unit");
    }
}

void Statistics::pushValue(Clock::duration time, uint64_t size) {
    static_assert(std::is_floating_point_v<Value>, "Need floating point type for the above cast to work properly");
    const Value timeSeconds = std::chrono::duration_cast<std::chrono::duration<Value>>(time).count();

    switch (getMeasurementUnit()) {
    case MeasurementUnit::Microseconds: {
        const Value timeMicroseconds = timeSeconds * 1e6;
        this->pushValue(timeMicroseconds);
        break;
    }
    case MeasurementUnit::GigabytesPerSecond: {
        const Value timeNanoseconds = timeSeconds * 1e9;
        const Value bandwidth = size / timeNanoseconds; // Bytes/Nanoseconds = Gigabytes/Seconds
        this->pushValue(bandwidth);
        break;
    }
    default:
        ERROR("Unknown measurement unit");
    }
}

void Statistics::pushValue(Value value) {
    ERROR_IF(samplesCount > maxSamplesCount, "Too much values pushed by the test");
    samples[samplesCount++] = value;

    if (std::isinf(value)) {
        this->reachedInfinity = true;
    }
}

bool Statistics::isEmpty() {
    return samplesCount == 0;
}

bool Statistics::isFull() {
    return samplesCount == maxSamplesCount;
}

struct ColumnInfo {
    int width;
    const char *label;
    bool printUnit;
};
const static ColumnInfo columns[] = {
    {getTestCaseNameColumnWidth(), "TestCase", false},
    {15, "Mean", true},
    {15, "Median", true},
    {15, "StdDev", false},
    {15, "Min", true},
    {15, "Max", true},
};
const static int columnCount = sizeof(columns) / sizeof(columns[0]);

static std::string getColumnName(const std::string &label, const std::string &unit, bool hasUnit) {
    if (hasUnit) {
        return label + " [" + unit + "]";
    }
    return label;
}

void Statistics::printStatisticsHeader(Configuration::PrintType printType) {
    std::string unit = "";
    switch (getMeasurementUnit()) {
    case MeasurementUnit::Microseconds:
        unit = "us";
        break;
    case MeasurementUnit::GigabytesPerSecond:
        unit = "GB/s";
        break;
    default:
        ERROR("Unknown measurement unit");
    }

    switch (printType) {
    case Configuration::PrintType::Verbose:
    case Configuration::PrintType::Default: {
        for (const ColumnInfo &column : columns) {
            std::cout << std::setw(column.width) << getColumnName(column.label, unit, column.printUnit);
        }
        std::cout << std::endl;
        break;
    }
    case Configuration::PrintType::Csv:
        for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
            const ColumnInfo &column = columns[columnIndex];
            std::cout << getColumnName(column.label, unit, column.printUnit);
            if (columnIndex != columnCount - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
        break;
    default:
        ERROR("unknown print type selected");
    }
}

void Statistics::printStatistics(const std::string &testCaseName) {
    if (this->reachedInfinity) {
        printStatisticsString(testCaseName, "inf");
    } else {
        switch (printType) {
        case Configuration::PrintType::Verbose:
        case Configuration::PrintType::Default: {
            int column = 0;
            std::cout << std::fixed;
            std::cout << std::setw(columns[column++].width) << testCaseName;
            std::cout << std::setw(columns[column++].width) << std::setprecision(3) << mean();
            std::cout << std::setw(columns[column++].width) << std::setprecision(3) << median();
            std::cout << std::setw(columns[column++].width - 1) << std::setprecision(2) << 100 * standardDeviation() << "%";
            std::cout << std::setw(columns[column++].width) << std::setprecision(3) << min();
            std::cout << std::setw(columns[column++].width) << std::setprecision(3) << max();
            std::cout << std::endl;
            break;
        }
        case Configuration::PrintType::Csv: {
            std::cout << std::fixed;
            std::cout << testCaseName << ",";
            std::cout << std::setprecision(3) << mean() << ",";
            std::cout << std::setprecision(3) << median() << ",";
            std::cout << std::setprecision(2) << 100 * standardDeviation() << "%,";
            std::cout << std::setprecision(3) << min() << ",";
            std::cout << std::setprecision(3) << max();
            std::cout << std::endl;
            break;
        }
        default:
            ERROR("unknown print type selected");
        }
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

void Statistics::printStatisticsString(const std::string &testCaseName, const std::string &message) {
    switch (printType) {
    case Configuration::PrintType::Verbose:
    case Configuration::PrintType::Default: {
        std::cout << std::setw(columns[0].width) << testCaseName;
        for (int column = 1; column < columnCount; column++) {
            std::cout << std::setw(columns[column].width) << message;
        }
        std::cout << std::endl;
        break;
    }
    case Configuration::PrintType::Csv: {
        std::cout << testCaseName << ",";
        for (int column = 1; column < columnCount; column++) {
            std::cout << message;
            if (column != columnCount - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
        break;
    }
    default:
        ERROR("unknown print type selected");
    }
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
        diffSum += difference * difference;
    }
    double stdDev = static_cast<double>(diffSum);
    stdDev /= samplesCount;
    stdDev = std::sqrt(static_cast<double>(stdDev));
    stdDev /= mean;
    return stdDev;
}
