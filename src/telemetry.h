#ifndef _TELEMETRY_H
#define _TELEMETRY_H

#include <inttypes.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#include "crc32.h"
#include "base40.h"

#define MAX_TELEMETRY_LEN     63
#define MAX_TELEMETRY_FIELDS  MAX_TELEMETRY_LEN - 3
#define MAX_TELEMETRY_BYTES   MAX_TELEMETRY_LEN * 4
#define BYTES_PER_FIELD       4

#define FIELD_START           0
#define FIELD_COUNT           1
#define FIELD_PACKET_NUMBER   2
#define FIELD_DATE            3
#define FIELD_TIME            4
#define FIELD_MILLISECOND     5
#define FIELD_MISSION_ID      6
#define FIELD_LATITUDE        7
#define FIELD_LONGITUDE       8
#define FIELD_ALTITUDE        9
#define FIELD_HEADING         10
#define FIELD_VARIABLE        11

#define START_MARKER          0xAA55AA55
#define END_MARKER            0xEEDDEEDD

typedef struct {
  uint8_t* data;
  uint8_t len;          /* in fields */
} telemetry_packet_t;

telemetry_packet_t* telemetry_create_packet(uint8_t number_fields);
uint8_t telemetry_write_field_uint32(telemetry_packet_t* packet, uint32_t data, uint8_t field_number);
uint8_t telemetry_write_field_int32(telemetry_packet_t* packet, int32_t data, uint8_t field_number);
uint8_t telemetry_write_field_float(telemetry_packet_t* packet, float data, uint8_t field_number);
uint32_t telemetry_read_field_uint32(telemetry_packet_t* packet, uint8_t field_number);
int32_t telemetry_read_field_int32(telemetry_packet_t* packet, uint8_t field_number);
float telemetry_read_field_float(telemetry_packet_t* packet, uint8_t field_number);
uint32_t telemetry_read_crc32(telemetry_packet_t* packet);


#endif
