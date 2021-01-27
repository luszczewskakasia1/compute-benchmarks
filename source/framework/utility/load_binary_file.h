#pragma once

#include <fstream>
#include <iostream>
#include <vector>

inline std::vector<uint8_t> loadBinaryFile(const std::string &filePath) {
    std::ifstream stream(filePath, std::ios::in | std::ios::binary);
    if (!stream.good()) {
        return {};
    }

    stream.seekg(0, stream.end);
    const size_t length = static_cast<size_t>(stream.tellg());
    stream.seekg(0, stream.beg);

    std::vector<uint8_t> binary_file(length);
    stream.read(reinterpret_cast<char *>(binary_file.data()), length);
    return binary_file;
}
