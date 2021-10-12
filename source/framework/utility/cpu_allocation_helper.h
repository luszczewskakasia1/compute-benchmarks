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

#include <cstddef>
#include <memory>

struct CpuAllocationHelper {
    class AlignedAllocation {
      public:
        AlignedAllocation(std::unique_ptr<std::byte[]> &&rawMemory, std::byte *alignedMemory);
        std::byte *get() { return alignedMemory; }
        operator std::byte *() { return alignedMemory; }

      private:
        std::unique_ptr<std::byte[]> rawMemory;
        std::byte *alignedMemory;
    };

    class MisalignedAllocation {
      public:
        MisalignedAllocation(AlignedAllocation &&alignedAllocation, std::byte *misalignedMemory);
        std::byte *get() { return misalignedMemory; }
        operator std::byte *() { return misalignedMemory; }

      private:
        AlignedAllocation alignedAllocation;
        std::byte *misalignedMemory;
    };

    static AlignedAllocation allocateAlignedAllocation(size_t size, size_t alignment);
    static MisalignedAllocation allocateMisalignedAllocation(size_t size, size_t alignment, size_t misalignment);
};
