/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021 Intel Corporation
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express or
 * implied warranties, other than those that are expressly stated in the License.
 */

#include "definitions/write_buffer_rect.h"

#include "framework/test_case/register_test_case.h"

#include <gtest/gtest.h>

static const inline RegisterTestCase<WriteBufferRect> registerTestCase{};

using Tuple = ThreeComponentUintArgument::TupleType;
class WriteBufferRectTestEntireBuffer : public ::testing::TestWithParam<std::tuple<bool, Tuple>> {
};

TEST_P(WriteBufferRectTestEntireBuffer, Test) {
    const auto region = std::get<1>(GetParam());
    const auto size = std::get<0>(region) * std::get<1>(region) * std::get<2>(region);

    WriteBufferRectArguments args;
    args.api = Api::OpenCL;
    args.size = size;
    args.compressed = std::get<0>(GetParam());
    args.origin = Tuple(0, 0, 0);
    args.region = std::get<1>(GetParam());
    args.rPitch = args.region[0];
    args.sPitch = args.region[0] * args.region[1];

    WriteBufferRect test;
    test.run(args);
}

INSTANTIATE_TEST_SUITE_P(
    WriteBufferRectTestEntireBuffer,
    WriteBufferRectTestEntireBuffer,
    ::testing::Combine(
        ::testing::Values(false, true),
        ::testing::Values(Tuple(128, 128, 1), Tuple(128, 128, 128), Tuple(128, 1024, 1024), Tuple(1024, 16, 1024), Tuple(1024, 1024, 16))));
