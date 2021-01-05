#pragma once

#include <memory>
#include <string>

struct TestCaseArgumentsBase;

// This class represent information specific to a specific benchmark, e.g. ulls_benchmark or memory_benchmark.
// It is used to configure the behaviour of some framework classes.
class BenchmarkInfo {
  private:
    static std::unique_ptr<BenchmarkInfo> instance;

  public:
    static BenchmarkInfo &get() { return *instance; }
    static void set(BenchmarkInfo *instance) { BenchmarkInfo::instance.reset(instance); }

    // General textual data
    virtual std::string getBenchmarkDescription() = 0;
    virtual std::string getBenchmarkName() = 0;
    std::string getBenchmarkFilename();

    // Unit of numbers that are returned by all tests in the given framework
    enum class MeasurementUnit {
        Microseconds,
        GigabytesPerSecond,
    };
    virtual MeasurementUnit getMeasurementUnit() = 0;

    // Width of the first column containing names of test cases.
    virtual int getTestCaseNameColumnWidth() = 0;

    struct BenchmarkSpecificConfigurationBase {};
    virtual std::unique_ptr<BenchmarkSpecificConfigurationBase> createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) = 0;
};

struct BenchmarkInfoImpl : BenchmarkInfo {
    std::string getBenchmarkDescription() override;
    std::string getBenchmarkName() override;
    virtual MeasurementUnit getMeasurementUnit() override;
    int getTestCaseNameColumnWidth() override;
    std::unique_ptr<BenchmarkSpecificConfigurationBase> createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments) override;
};
