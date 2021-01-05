#pragma once

#include "benchmark_info.h"

std::unique_ptr<BenchmarkInfo> BenchmarkInfo::create() {
    return std::unique_ptr<BenchmarkInfo>();
}

BenchmarkInfo &BenchmarkInfo::get() {
    static std::unique_ptr<BenchmarkInfo> benchmarkInfo = {};
    if (benchmarkInfo == nullptr) {
        benchmarkInfo = BenchmarkInfo::create();
    }
    return *benchmarkInfo;
}

std::string BenchmarkInfo::getBenchmarkFilename() {
#ifdef WIN32
    return getBenchmarkName() + ".exe";
#else
    return getBenchmarkName();
#endif
}
