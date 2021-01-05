#pragma once

#include "benchmark_info.h"

std::unique_ptr<BenchmarkInfo> BenchmarkInfo::instance = {};

std::string BenchmarkInfo::getBenchmarkFilename() {
#ifdef WIN32
    return getBenchmarkName() + ".exe";
#else
    return getBenchmarkName();
#endif
}
