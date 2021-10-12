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

#pragma once

#include "framework/ocl/cl.h"
#include "framework/utility/buffer_contents_helper.h"

class BufferContentsHelperOcl : public BufferContentsHelper {
  public:
    static cl_int fillBuffer(cl_command_queue queue, cl_mem buffer, size_t bufferSize, BufferContents contents);
    static cl_int fillUsmBuffer(cl_command_queue queue, void *usmBuffer, size_t bufferSize, BufferContents contents);

  private:
    static cl_int fillBufferWithRandomBytes(cl_command_queue queue, cl_mem buffer, size_t bufferSize);
    static cl_int fillBufferWithZeros(cl_command_queue queue, cl_mem buffer, size_t bufferSize);
    static cl_int fillBufferWithIncreasingBytes(cl_command_queue queue, cl_mem buffer, size_t bufferSize);

    static cl_int fillUsmBufferWithRandomBytes(cl_command_queue queue, void *usmBuffer, size_t bufferSize);
    static cl_int fillUsmBufferWithZeros(cl_command_queue queue, void *usmBuffer, size_t bufferSize);
};
