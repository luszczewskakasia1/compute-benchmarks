/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2021-2022 Intel Corporation
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

// #include <CL/sycl.hpp>

// #include "framework/ocl/opencl.h"
// #include "framework/ocl/utility/profiling_helper.h"

#include "framework/sycl/sycl.h"
#include "framework/test_case/register_test_case.h"
#include "framework/utility/timer.h"

#include "definitions/matrix_multiply.h"

using namespace cl::sycl;
using namespace cl::sycl::access;
using sycl_buffer_t = cl::sycl::buffer<float, 1>;

static TestResult run(const MatrixMultiplyArguments &arguments, Statistics &statistics) {
    size_t buf_count = 2;
    size_t elem_count = 4;
    cl::sycl::queue q;
    std::vector<void *> send_bufs;
    std::vector<void *> recv_bufs;
    std::vector<float *> host_buf;
    send_bufs.resize(buf_count);
    recv_bufs.resize(buf_count);
    host_buf.resize(buf_count);

    for (size_t idx = 0; idx < buf_count; idx++) {
        send_bufs[idx] = new cl::sycl::buffer<float, 1>(elem_count);
        recv_bufs[idx] = new cl::sycl::buffer<float, 1>(elem_count);
        host_buf[idx] = new float[elem_count];
    }

    for (size_t b_idx = 0; b_idx < buf_count; b_idx++) {
        q.submit([&](handler &cgh) {
            auto send_buf = (static_cast<sycl_buffer_t *>(send_bufs[b_idx]));
            auto recv_buf = (static_cast<sycl_buffer_t *>(recv_bufs[b_idx]));
            auto send_buf_acc = send_buf->get_access<mode::write>(cgh);
            auto recv_buf_acc = recv_buf->get_access<mode::write>(cgh);
            cgh.parallel_for<class reduce_buf_fill>(range<1>{elem_count}, [=](item<1> e_idx) {
                send_buf_acc[e_idx] = 1;
                recv_buf_acc[e_idx] = 0;
            });
        });
    }

    for (size_t b_idx = 0; b_idx < buf_count; b_idx++) {
        auto send_buf = (static_cast<sycl_buffer_t *>(send_bufs[b_idx]));
        auto recv_buf = (static_cast<sycl_buffer_t *>(recv_bufs[b_idx]));
        auto send_buf_acc = send_buf->get_access<mode::read>();
        auto recv_buf_acc = recv_buf->get_access<mode::write>();
        printf("1 memcopy start host_buf ptr %p, send_buf ptr %p\n", host_buf[b_idx], (void *)send_buf_acc.get_pointer());
        fflush(stdout);
        memcpy(host_buf[b_idx], send_buf_acc.get_pointer(), elem_count * sizeof(float));
        printf("2 memcopy start\n");
        fflush(stdout);
        memcpy(recv_buf_acc.get_pointer(), host_buf[b_idx], elem_count * sizeof(float));
    }

    return TestResult::Success;
}

static RegisterTestCaseImplementation<MatrixMultiply> registerTestCase(run, Api::SYCL);
