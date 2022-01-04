#include "crc32.h"

// standard crc32b algorithm
uint32_t crc32(const uint8_t *data, uint32_t len) {
    uint32_t crc = 0xFFFFFFFF;

    for(uint32_t i = 0; i < len; i++) {
        uint8_t ch = data[i];
        for(uint32_t j = 0; j < 8; j++) {
            uint32_t b = (ch ^ crc) & 1;
            crc >>= 1;
            if(b) crc = crc ^ 0xEDB88320;
            ch >>= 1;
        }
    }

    return ~crc;
}

