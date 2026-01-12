/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025-2026 Intel Corporation
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

#include "framework/additional/embargo/additional_configuration.h"
#include "framework/configuration.h"
#include "framework/embargo/cuda/cuda.h"

namespace CUDA {

static void printDeviceInfo() {
    int deviceCount = 0;
    cudaGetDeviceCount(&deviceCount);

    if (deviceCount == 0) {
        std::cout << "CUDA devices: NONE" << std::endl
                  << std::endl;
        return;
    }

    int deviceId = 0;
    EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGetDevice(&deviceId));

    cudaDeviceProp deviceProperties{};
    EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGetDeviceProperties(&deviceProperties, deviceId));
    int clockRateKHz = 0;
    EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaDeviceGetAttribute(&clockRateKHz, cudaDevAttrClockRate, deviceId));

    std::cout << "CUDA device information" << std::endl;
    std::cout << "\tCUDA API type:             " << (Configuration::get().additionalConfiguration.cudaApiType == CudaApiType::Runtime ? "Runtime" : "Driver") << std::endl;
    std::cout << "\tCompute capability:        " << deviceProperties.major << "." << deviceProperties.minor << std::endl;
    std::cout << "\tDevice name:               " << deviceProperties.name << std::endl;
    std::cout << "\tMax threads per block:     " << deviceProperties.maxThreadsPerBlock << std::endl;
    std::cout << "\tRegisters per block:       " << deviceProperties.regsPerBlock << std::endl;
    std::cout << "\tClock rate:                " << clockRateKHz << " KHz" << std::endl;
    std::cout << std::endl;
}

static void printAvailableDevices() {
    int deviceCount = 0;
    cudaGetDeviceCount(&deviceCount);

    if (deviceCount == 0) {
        std::cout << "CUDA devices: NONE" << std::endl
                  << std::endl;
        return;
    }

    std::cout << "CUDA devices: " << deviceCount << std::endl;

    for (int deviceId = 0; deviceId < deviceCount; ++deviceId) {
        cudaDeviceProp deviceProperties{};
        EXPECT_CUDA_RUNTIME_RESULT_SUCCESS(cudaGetDeviceProperties(&deviceProperties, deviceId));

        std::cout << "\tDevice: " << deviceProperties.name << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

} // namespace CUDA
