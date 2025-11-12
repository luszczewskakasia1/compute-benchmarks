/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (C) 2025 Intel Corporation
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

#include <iomanip>

namespace CUDA {

static void printDeviceInfo() {
    if (Configuration::get().additionalConfiguration.cudaApiType == CudaApiType::Runtime) {
        int deviceId = 0;
        cudaDeviceProp deviceProp;

        cudaSetDevice(deviceId);
        cudaGetDeviceProperties(&deviceProp, deviceId);

        int clockRate;
        cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, 0);

        std::cout << "CUDA runtime version: " << deviceProp.major << "." << deviceProp.minor << std::endl;

        std::cout << "\tDevice: " << deviceProp.name << std::endl;
        std::cout << "\t\tMax threads per block:  " << deviceProp.maxThreadsPerBlock << std::endl;
        std::cout << "\t\tRegisters per block:    " << deviceProp.regsPerBlock << std::endl;
        std::cout << "\t\tClock Rate:         " << clockRate << " KHz" << std::endl;
    } else {
        CUdevice device;
        CUresult result = cuInit(0);
        if (result != CUDA_SUCCESS) {
            std::cerr << "Failed to initialize CUDA Driver API" << std::endl;
            return;
        }

        result = cuDeviceGet(&device, 0);
        if (result != CUDA_SUCCESS) {
            std::cerr << "Failed to get CUDA device" << std::endl;
            return;
        }

        char name[256];
        cuDeviceGetName(name, 256, device);

        int major = 0, minor = 0;
        cuDeviceGetAttribute(&major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device);
        cuDeviceGetAttribute(&minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, device);

        int maxThreadsPerBlock = 0;
        cuDeviceGetAttribute(&maxThreadsPerBlock, CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, device);

        int regsPerBlock = 0;
        cuDeviceGetAttribute(&regsPerBlock, CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK, device);

        int clockRate = 0;
        cuDeviceGetAttribute(&clockRate, CU_DEVICE_ATTRIBUTE_CLOCK_RATE, device);

        std::cout << "CUDA driver API version: " << major << "." << minor << std::endl;
        std::cout << "\tDevice: " << name << std::endl;
        std::cout << "\t\tMax threads per block:  " << maxThreadsPerBlock << std::endl;
        std::cout << "\t\tRegisters per block:    " << regsPerBlock << std::endl;
        std::cout << "\t\tClock Rate:             " << clockRate << " KHz" << std::endl;
    }

    std::cout << std::endl;
}

static void printAvailableDevices() {
    int deviceCount = 0;
    cudaError_t err = cudaGetDeviceCount(&deviceCount);

    if (err != cudaSuccess || deviceCount == 0) {
        std::cout << "CUDA devices: NONE" << std::endl;
        return;
    }

    std::cout << "CUDA devices: " << deviceCount << std::endl;

    for (int i = 0; i < deviceCount; ++i) {
        cudaDeviceProp prop;
        cudaGetDeviceProperties(&prop, i);

        std::cout << "\tDevice: " << prop.name << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

} // namespace CUDA
