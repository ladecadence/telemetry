#ifndef _TELEMETRY_H
#define _TELEMETRY_H

#include <inttypes.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#include "crc32.h"
#include "base40.h"

#define TELEMETRY_MAX_LEN               63
#define TELEMETRY_BYTES_PER_FIELD       4
#define TELEMETRY_MAX_FIELDS            TELEMETRY_MAX_LEN - 3
#define TELEMETRY_MAX_BYTES             TELEMETRY_MAX_BYTES * TELEMETRY_BYTES_PER_FIELD
#define TELEMETRY_STANDARD_MIN_FIELDS   10

#define TELEMETRY_FIELD_START           0
#define TELEMETRY_FIELD_COUNT           1
#define TELEMETRY_FIELD_PACKET_NUMBER   2
#define TELEMETRY_FIELD_DATE            3
#define TELEMETRY_FIELD_TIME            4
#define TELEMETRY_FIELD_MILLISECOND     5
#define TELEMETRY_FIELD_MISSION_ID      6
#define TELEMETRY_FIELD_LATITUDE        7
#define TELEMETRY_FIELD_LONGITUDE       8
#define TELEMETRY_FIELD_ALTITUDE        9
#define TELEMETRY_FIELD_HEADING         10
#define TELEMETRY_FIELD_VARIABLE        11

#define TELEMETRY_START_MARKER          0xAA55AA55
#define TELEMETRY_END_MARKER            0xEEDDEEDD

typedef struct {
  uint8_t* data;
  uint8_t len;          /* in fields */
} telemetry_packet_t;

telemetry_packet_t* telemetry_create_packet(uint8_t number_fields);
void telemetry_delete_packet(telemetry_packet_t* packet);
uint8_t telemetry_write_field_uint32(telemetry_packet_t* packet, uint32_t data, uint8_t field_number);
uint8_t telemetry_write_field_int32(telemetry_packet_t* packet, int32_t data, uint8_t field_number);
uint8_t telemetry_write_field_float(telemetry_packet_t* packet, float data, uint8_t field_number);
uint32_t telemetry_read_field_uint32(telemetry_packet_t* packet, uint8_t field_number);
int32_t telemetry_read_field_int32(telemetry_packet_t* packet, uint8_t field_number);
float telemetry_read_field_float(telemetry_packet_t* packet, uint8_t field_number);
uint32_t telemetry_read_crc32(telemetry_packet_t* packet);


#endif
