#include "benchmark_info.h"

std::unique_ptr<BenchmarkInfo> BenchmarkInfo::instance = {};

BenchmarkInfo &BenchmarkInfo::get() {
    return *BenchmarkInfo::instance;
}

void BenchmarkInfo::set(BenchmarkInfo *instance) {
    BenchmarkInfo::instance.reset(instance);
}

std::string BenchmarkInfo::getBenchmarkFilename() {
#ifdef WIN32
    return getBenchmarkName() + ".exe";
#else
    return getBenchmarkName();
#endif
}
