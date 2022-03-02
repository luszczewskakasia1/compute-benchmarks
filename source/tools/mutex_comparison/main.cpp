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

#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <numeric>
#include <shared_mutex>
#include <vector>

constexpr uint32_t comparisonIterations = 100;
constexpr uint32_t lockIterations = 100000;

void printMutexTimes(std::vector<double> &vec) {
    auto sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    auto avg = sum / comparisonIterations;
    auto [minIt, maxIt] = std::minmax_element(vec.begin(), vec.end());
    auto min = *minIt;
    auto max = *maxIt;
    std::cout << "Min: " << min << " ns, Max: " << max << " ns, Avg: " << avg << " ns" << std::endl;
}

int main() {
    std::shared_mutex sharedMtx;
    std::mutex mtx;

    std::vector<double> sharedMtxSharedTimes;
    std::vector<double> sharedMtxExclusiveTimes;
    std::vector<double> mtxTimes;

    std::cout << "--- Comparison start ---" << std::endl;

    for (uint32_t j = 0; j < comparisonIterations; j++) {
        auto start = std::chrono::steady_clock::now();
        for (uint32_t i = 0; i < lockIterations; i++) {
            sharedMtx.lock_shared();
            sharedMtx.unlock_shared();
        }
        auto end = std::chrono::steady_clock::now();
        sharedMtxSharedTimes.push_back((std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / static_cast<double>(lockIterations));

        start = std::chrono::steady_clock::now();
        for (uint32_t i = 0; i < lockIterations; i++) {
            sharedMtx.lock();
            sharedMtx.unlock();
        }
        end = std::chrono::steady_clock::now();
        sharedMtxExclusiveTimes.push_back((std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / static_cast<double>(lockIterations));

        start = std::chrono::steady_clock::now();
        for (uint32_t i = 0; i < lockIterations; i++) {
            mtx.lock();
            mtx.unlock();
        }
        end = std::chrono::steady_clock::now();
        mtxTimes.push_back((std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / static_cast<double>(lockIterations));
    }

    std::cout << "Shared mutex - shared lock:  ";
    printMutexTimes(sharedMtxSharedTimes);
    std::cout << "Shared mutex exclusive lock: ";
    printMutexTimes(sharedMtxExclusiveTimes);
    std::cout << "Mutex - exclusive lock:      ";
    printMutexTimes(mtxTimes);

    return 0;
}