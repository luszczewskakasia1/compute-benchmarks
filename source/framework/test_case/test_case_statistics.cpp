#include "test_case_statistics.h"

#include "framework/benchmark_info.h"
#include "framework/utility/error.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <type_traits>

TestCaseStatistics::TestCaseStatistics(size_t maxSamplesCount, Configuration::PrintType printType)
    : Statistics(maxSamplesCount),
      printType(printType) {
    this->samplesMap[""] = {};
}

void TestCaseStatistics::pushValue(Clock::duration time, const std::string &description) {
    static_assert(std::is_floating_point_v<Value>, "Need floating point type for the above cast to work properly");
    const Value timeSeconds = std::chrono::duration_cast<std::chrono::duration<Value>>(time).count();

    switch (BenchmarkInfo::get().getMeasurementUnit()) {
    case BenchmarkInfo::MeasurementUnit::Microseconds: {
        const Value timeMicroseconds = timeSeconds * 1e6;
        this->pushValue(timeMicroseconds, description);
        break;
    }
    case BenchmarkInfo::MeasurementUnit::GigabytesPerSecond:
        ERROR("Buffer size needs to be passed in bandwidth mode");
    default:
        ERROR("Unknown measurement unit");
    }
}

void TestCaseStatistics::pushValue(Clock::duration time, uint64_t size, const std::string &description) {
    static_assert(std::is_floating_point_v<Value>, "Need floating point type for the above cast to work properly");
    const Value timeSeconds = std::chrono::duration_cast<std::chrono::duration<Value>>(time).count();

    switch (BenchmarkInfo::get().getMeasurementUnit()) {
    case BenchmarkInfo::MeasurementUnit::Microseconds: {
        const Value timeMicroseconds = timeSeconds * 1e6;
        this->pushValue(timeMicroseconds, description);
        break;
    }
    case BenchmarkInfo::MeasurementUnit::GigabytesPerSecond: {
        const Value timeNanoseconds = timeSeconds * 1e9;
        const Value bandwidth = size / timeNanoseconds; // Bytes/Nanoseconds = Gigabytes/Seconds
        this->pushValue(bandwidth, description);
        break;
    }
    default:
        ERROR("Unknown measurement unit");
    }
}

bool TestCaseStatistics::isEmpty() const {
    for (auto &samplesEntry : samplesMap) {
        if (samplesEntry.second.size() != 0) {
            return false;
        }
    }
    return true;
}

bool TestCaseStatistics::isFull() const {
    for (auto &samplesEntry : samplesMap) {
        if (samplesEntry.second.size() != maxSamplesCount) {
            return false;
        }
    }
    return true;
}

void TestCaseStatistics::pushValue(Value value, const std::string &description) {
    auto &samples = this->samplesMap[description];
    ERROR_IF(samples.size() == maxSamplesCount, "Too many values pushed by the test");

    samples.push_back(value);
    if (std::isinf(value)) {
        this->reachedInfinity = true;
    }
}

struct ColumnInfo {
    int width;
    const char *label;
    bool printUnit;

    static constexpr size_t getColumnCount() { return 6; }
    static std::array<ColumnInfo, 6> getColumns() {
        return {{
            {BenchmarkInfo::get().getTestCaseNameColumnWidth(), "TestCase", false},
            {15, "Mean", true},
            {15, "Median", true},
            {15, "StdDev", false},
            {15, "Min", true},
            {15, "Max", true},
        }};
    }
};

static std::string getColumnName(const std::string &label, const std::string &unit, bool hasUnit) {
    if (hasUnit) {
        return label + " [" + unit + "]";
    }
    return label;
}

void TestCaseStatistics::printStatisticsHeader(Configuration::PrintType printType) {
    std::string unit = "";
    switch (BenchmarkInfo::get().getMeasurementUnit()) {
    case BenchmarkInfo::MeasurementUnit::Microseconds:
        unit = "us";
        break;
    case BenchmarkInfo::MeasurementUnit::GigabytesPerSecond:
        unit = "GB/s";
        break;
    default:
        ERROR("Unknown measurement unit");
    }

    const auto columns = ColumnInfo::getColumns();
    const auto columnCount = ColumnInfo::getColumnCount();
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

void TestCaseStatistics::printStatistics(const std::string &testCaseName) {
    const auto columns = ColumnInfo::getColumns();
    const auto columnCount = ColumnInfo::getColumnCount();

    const Metrics metrics{samplesMap[""]};
    const MetricsStrings metricsStrings{metrics, this->reachedInfinity};
    switch (printType) {
    case Configuration::PrintType::Verbose:
    case Configuration::PrintType::Default: {
        int column = 0;
        std::cout << std::setw(columns[column++].width) << testCaseName;
        std::cout << std::setw(columns[column++].width) << metricsStrings.mean;
        std::cout << std::setw(columns[column++].width) << metricsStrings.median;
        std::cout << std::setw(columns[column++].width) << metricsStrings.standardDeviation;
        std::cout << std::setw(columns[column++].width) << metricsStrings.min;
        std::cout << std::setw(columns[column++].width) << metricsStrings.max;
        std::cout << std::endl;
        break;
    }
    case Configuration::PrintType::Csv: {
        std::cout << testCaseName << ",";
        std::cout << metricsStrings.mean << ",";
        std::cout << metricsStrings.median << ",";
        std::cout << metricsStrings.standardDeviation << ",";
        std::cout << metricsStrings.min << ",";
        std::cout << metricsStrings.max;
        std::cout << std::endl;
        break;
    }
    default:
        ERROR("unknown print type selected");
    }

    if (printType == Configuration::PrintType::Verbose) {
        for (const auto &entry : this->samplesMap) {
            const auto &samples = entry.first;
            const auto samplesCount = entry.second.size();

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
}

void TestCaseStatistics::printStatisticsString(const std::string &testCaseName, const std::string &message) {
    const auto columns = ColumnInfo::getColumns();
    const auto columnCount = ColumnInfo::getColumnCount();
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

TestCaseStatistics::Metrics::Metrics(const SamplesVector &samples)
    : min(calculateMin(samples)),
      max(calculateMax(samples)),
      mean(calculateMean(samples)),
      median(calculateMedian(samples)),
      standardDeviation(calculateStandardDeviation(samples, mean)) {}

TestCaseStatistics::Value TestCaseStatistics::Metrics::calculateMin(const SamplesVector &samples) {
    return *std::min_element(samples.begin(), samples.end());
}

TestCaseStatistics::Value TestCaseStatistics::Metrics::calculateMax(const SamplesVector &samples) {
    return *std::max_element(samples.begin(), samples.end());
}

TestCaseStatistics::Value TestCaseStatistics::Metrics::calculateMean(const SamplesVector &samples) {
    return std::accumulate(samples.begin(), samples.end(), Value{0}) / samples.size();
}

TestCaseStatistics::Value TestCaseStatistics::Metrics::calculateMedian(const SamplesVector &samples) {
    SamplesVector sortedSamples = samples;
    std::sort(sortedSamples.begin(), sortedSamples.end());
    const auto samplesCount = sortedSamples.size();
    if (samplesCount % 2 == 0) {
        const auto left = sortedSamples[samplesCount / 2 - 1];
        const auto right = sortedSamples[samplesCount / 2];
        return (left + right) / 2;
    } else {
        const auto middle = sortedSamples[samplesCount / 2];
        return middle;
    }
}

TestCaseStatistics::Value TestCaseStatistics::Metrics::calculateStandardDeviation(const SamplesVector &samples, Value mean) {
    const auto samplesCount = samples.size();
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

TestCaseStatistics::MetricsStrings::MetricsStrings(const Metrics &metrics, bool reachedInfinity)
    : min(generateMin(metrics.min)),
      max(generateMax(metrics.max)),
      mean(generateMean(metrics.mean, reachedInfinity)),
      median(generateMedian(metrics.median)),
      standardDeviation(generateStandardDeviation(metrics.standardDeviation, reachedInfinity)) {
}

std::string TestCaseStatistics::MetricsStrings::generateMin(Value min) {
    return generate(min);
}

std::string TestCaseStatistics::MetricsStrings::generateMax(Value max) {
    return generate(max);
}

std::string TestCaseStatistics::MetricsStrings::generateMean(Value mean, bool reachedInfinity) {
    if (reachedInfinity) {
        return "inf";
    }
    return generate(mean);
}

std::string TestCaseStatistics::MetricsStrings::generateMedian(Value median) {
    return generate(median);
}

std::string TestCaseStatistics::MetricsStrings::generateStandardDeviation(Value standardDeviation, bool reachedInfinity) {
    if (reachedInfinity) {
        return "inf";
    }

    std::ostringstream result{};
    result << std::fixed << std::setprecision(2) << (100 * standardDeviation) << "%";
    return result.str();
}

std::string TestCaseStatistics::MetricsStrings::generate(Value value) {
    std::ostringstream result{};
    result << std::fixed << std::setprecision(3) << value;
    return result.str();
}
