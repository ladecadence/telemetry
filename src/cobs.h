#ifndef __COBS_H
#define __COBS_H

#include <inttypes.h>

uint16_t cobs_encode(uint8_t *data, uint16_t length, uint8_t *buffer);
uint16_t cobs_decode(const uint8_t *buffer, uint16_t length, uint8_t *data);

#endif
