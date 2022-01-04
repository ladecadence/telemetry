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
#define TELEMETRY_STANDARD_MIN_FIELDS   12

#define TELEMETRY_FIELD_START           0
#define TELEMETRY_FIELD_COUNT           1
#define TELEMETRY_FIELD_PACKET_NUMBER   2
#define TELEMETRY_FIELD_PACKET_TYPE     3
#define TELEMETRY_FIELD_DATE            4
#define TELEMETRY_FIELD_TIME            5
#define TELEMETRY_FIELD_MILLISECOND     6
#define TELEMETRY_FIELD_MISSION_ID      7
#define TELEMETRY_FIELD_LATITUDE        8
#define TELEMETRY_FIELD_LONGITUDE       9
#define TELEMETRY_FIELD_ALTITUDE        10
#define TELEMETRY_FIELD_HEADING         11
#define TELEMETRY_FIELD_VARIABLE        12

#define TELEMETRY_TYPE_TELEMETRY        0x77777777

#define TELEMETRY_START_MARKER          0xAA55AA55
#define TELEMETRY_END_MARKER            0xEEDDEEDD


/* Network byte order */

#define __bswap_32(x) \
     ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |                \
      (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#if __BYTE_ORDER == __BIG_ENDIAN
#define ntohl(x) x
#define htonl(x) x
#else
# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define ntohl(x)  __bswap_32 (x)
#  define htonl(x)  __bswap_32 (x)
# endif
#endif

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
