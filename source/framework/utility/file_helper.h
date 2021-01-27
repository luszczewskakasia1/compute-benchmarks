#pragma once

#include <iostream>
#include <vector>

struct FileHelper {
    static std::vector<uint8_t> loadFile(const std::string &filePath, std::ios_base::openmode openMode);
    static std::vector<uint8_t> loadBinaryFile(const std::string &filePath);
    static std::vector<uint8_t> loadTextFile(const std::string &filePath);
};
