#include "memory_benchmark/definitions/read_device_mem_buffer.h"

#include <gtest/gtest.h>

class ReadBufferDevMemTest : public ::testing::TestWithParam<std::tuple<size_t, bool>> {
};

TEST_P(ReadBufferDevMemTest, Test) {
    ReadDeviceMemBufferArguments args;
    args.api = Api::OpenCL;
    args.size = std::get<0>(GetParam());
    args.compressed = std::get<1>(GetParam());

    ReadDeviceMemBuffer test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    ReadBufferDevMemTest,
    ReadBufferDevMemTest,
    ::testing::Combine(
        ::testing::Values(64*kiloByte, 128*kiloByte, 512*kiloByte, 4*megaByte, 8*megaByte, 32*megaByte, 128*megaByte),
        ::testing::Values(false, true)));
