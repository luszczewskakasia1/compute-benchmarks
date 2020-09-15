#include <gtest/gtest.h>

namespace CommonGtestArgs {
inline auto allApis() {
    return ::testing::Values(Api::OpenCL, Api::L0);
}

inline auto workgroupCount() {
    return ::testing::Values(1, 100, 1000, 10000);
}

inline auto workgroupSize() {
    return ::testing::Values(1, 16, 32, 256);
}
} // namespace CommonGtestArgs
