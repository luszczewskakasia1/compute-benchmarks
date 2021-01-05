#pragma once

#include <memory>
#include <string>

struct TestCaseArgumentsBase;

// This class represent information specific to a specific benchmark, e.g. ulls_benchmark or memory_benchmark.
// It is used to configure the behaviour of some framework classes.
class BenchmarkInfo {
  private:
    static std::unique_ptr<BenchmarkInfo> create();

  public:
    static BenchmarkInfo &get();

    // Test map allows finding all tests that are present and indexing them by name

    // General textual data
    std::string getBenchmarkName();
    std::string getBenchmarkFilename();
    std::string getBenchmarkDescription();

    // Unit of numbers that are returned by all tests in the given framework
    enum class MeasurementUnit {
        Microseconds,
        GigabytesPerSecond,
    };
    MeasurementUnit getMeasurementUnit();

    // Width of the first column containing names of test cases.
    int getTestCaseNameColumnWidth();

    struct BenchmarkSpecificConfigurationBase {};
    std::unique_ptr<BenchmarkSpecificConfigurationBase> createBenchmarkSpecificConfiguration(TestCaseArgumentsBase &testCaseArguments);
};
