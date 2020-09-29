#include "framework/utility/random_helper.h"

thread_local std::mt19937 RandomHelper::generator{std::random_device{}()};

uint8_t RandomHelper::randomByte() {
    const uint32_t result = std::uniform_int_distribution<uint32_t>{0x00, 0xFF}(RandomHelper::generator);
    return static_cast<uint8_t>(result);
}

void RandomHelper::fillWithRandomBytes(uint8_t *buffer, size_t size) {
    const auto bufferEnd = buffer + size;
    for (; buffer < bufferEnd; buffer++) {
        *buffer = randomByte();
    }
}
