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

#if __has_include(<filesystem>)
#include <filesystem>
namespace FileSystem = std::filesystem;
#else
#include <experimental/filesystem>
namespace FileSystem = std::experimental::filesystem;
#endif

#include <filesystem>
#include <string>

struct WorkingDirectoryHelper {
    static FileSystem::path getExeLocation(); // OS-specific implementation

    static void changeDirectory(const FileSystem::path &directory) {
        FileSystem::current_path(directory);
    }

    static void changeDirectoryToExeDirectory() {
        const FileSystem::path exeLocation = getExeLocation();
        const FileSystem::path exeDirectory = exeLocation.parent_path();
        changeDirectory(exeDirectory);
    }
};
