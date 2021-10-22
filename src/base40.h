#ifndef __BASE40_H
#define __BASE40_H

#include <inttypes.h>

#define MAX_MISSION_ID  6

uint32_t encode_mission_id(char* mission_id);
char* decode_mission_id(char* mission_id, uint32_t code);

#endif
