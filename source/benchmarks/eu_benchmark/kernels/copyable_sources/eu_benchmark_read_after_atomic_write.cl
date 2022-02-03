/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2022 Intel Corporation
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

unsigned int shuffleIds(unsigned int id,
                        __local unsigned int *localMemCounter,
                        __local unsigned int *localMem) {
    if (get_local_id(0) == 0) {
        localMemCounter = 0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    localMem[atomic_inc(localMemCounter)] = id;
    barrier(CLK_LOCAL_MEM_FENCE);
    return atomic_dec(localMemCounter);
}

__kernel void read_after_atomic_write(__global unsigned int *buffer,
                                      uint useAtomic,
                                      uint shuffleRead,
                                      uint iterations) {
    const unsigned int id = get_global_id(0);
    __local unsigned int localMemCounter;
    __local unsigned int localMem[512];

    unsigned int value = 3;
    for (int i = 0; i < iterations; i++) {
        // Prepare IDs
        unsigned int idForRead = id;
        if (shuffleRead) {
            idForRead = shuffleIds(idForRead, &localMemCounter, localMem);
        }
        const unsigned int idForWrite = id;

        // Perform write
        if (useAtomic) {
            atomic_add(&buffer[idForWrite], value);
        } else {
            buffer[idForWrite] += value;
        }

        // Perform read
        value = buffer[idForRead];

        // Synchronize
        barrier(CLK_LOCAL_MEM_FENCE);
    }
}
