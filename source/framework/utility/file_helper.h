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

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

struct FileHelper {
    static std::vector<uint8_t> loadFile(const std::string &filePath, std::ios_base::openmode openMode);
    static std::vector<uint8_t> loadBinaryFile(const std::string &filePath);
    static std::vector<uint8_t> loadTextFile(const std::string &filePath);

    class FileOrConsole {
      public:
        FileOrConsole(const std::string &filePath, std::ios::openmode openMode, std::ostream &fallback);
        std::ostream &get();
        bool hasOwnedFile() const { return ownedFile != nullptr; }

      private:
        std::ostream &fallback;
        std::unique_ptr<std::ofstream> ownedFile = {};
    };
};
