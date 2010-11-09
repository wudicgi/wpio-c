#include <stdint.h>
#include "crc32.h"

uint32_t crc32(void *data, int length) {
    uint32_t *ptr = (uint32_t*)data;
    uint32_t *ptr_end = (uint32_t*)(data + length);

    uint32_t crc = 0;

    crc ^= CRC_SEED;

    while (ptr < ptr_end) {
        crc = crc32_table[(crc ^ *(ptr++)) & 0xFF] ^ (crc >> 8);
    }

    crc ^= CRC_SEED;

    return crc;
}
