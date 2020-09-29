#pragma once

#include <random>

namespace RandomHelper {
extern thread_local std::mt19937 generator;

uint8_t randomByte();
void fillWithRandomBytes(uint8_t *buffer, size_t size);
} // namespace RandomHelper
