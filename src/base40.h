#ifndef __BASE40_H
#define __BASE40_H

#include <inttypes.h>

#define BASE40_FIELD_MAX_CHARS  6

uint32_t base40_encode(char* data);
char* base40_decode(char* data, uint32_t code);

#endif
