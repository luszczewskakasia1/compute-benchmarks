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

#include "framework/l0/levelzero.h"
#include "framework/utility/buffer_contents_helper.h"

class BufferContentsHelperL0 : public BufferContentsHelper {
  public:
    static ze_result_t fillBuffer(ze_device_handle_t device, ze_context_handle_t context, ze_command_queue_handle_t queue,
                                  uint32_t queueOrdinal, void *buffer, size_t bufferSize, BufferContents contents);

    static ze_result_t fillBuffer(LevelZero &levelzero, void *buffer, size_t bufferSize, BufferContents contents);

  private:
    static ze_result_t fillBufferWithRandomBytes(ze_device_handle_t device, ze_context_handle_t context, ze_command_queue_handle_t queue,
                                                 uint32_t queueOrdinal, void *buffer, size_t bufferSize);

    static ze_result_t fillBufferWithZeros(ze_device_handle_t device, ze_context_handle_t context, ze_command_queue_handle_t queue,
                                           uint32_t queueOrdinal, void *buffer, size_t bufferSize);
};
