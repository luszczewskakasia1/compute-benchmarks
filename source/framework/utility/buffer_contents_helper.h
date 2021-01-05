#pragma once

#include <random>

class BufferContentsHelper {
  public:
    static void fillWithRandomBytes(uint8_t *buffer, size_t size);

  private:
    static thread_local std::mt19937 generator;
    static std::vector<uint8_t> cachedRandomData;

    static uint64_t randomOword();
};
