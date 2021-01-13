#include "benchmark_info.h"

std::unique_ptr<BenchmarkInfo> BenchmarkInfo::instance = {};

BenchmarkInfo &BenchmarkInfo::get() {
    return *BenchmarkInfo::instance;
}

bool BenchmarkInfo::isSet() {
    return BenchmarkInfo::instance.get() != nullptr;
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
