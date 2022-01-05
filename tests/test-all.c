#include <stdio.h>
#include "../src/telemetry.h"

int main(void) {
    telemetry_packet_t* telem;
    char base40_data[7];
    uint32_t uintdata;
    int32_t intdata;
    float floatdata;
    
    /* create packet */
    telem = telemetry_create_packet(TELEMETRY_STANDARD_MIN_FIELDS);
    
    
    /* insert data */
    telemetry_write_field_uint32(telem, 1, TELEMETRY_FIELD_PACKET_NUMBER);
    telemetry_write_field_uint32(telem,  base40_encode("010922"), TELEMETRY_FIELD_DATE);
    telemetry_write_field_uint32(telem,  base40_encode("123324"), TELEMETRY_FIELD_TIME);
    telemetry_write_field_uint32(telem, 102030, TELEMETRY_FIELD_MILLISECOND);
    telemetry_write_field_uint32(telem,  base40_encode("EKI3"), TELEMETRY_FIELD_MISSION_ID);
    telemetry_write_field_float(telem, 43.31653976658865, TELEMETRY_FIELD_LATITUDE);
    telemetry_write_field_float(telem, -1.9752830550936795, TELEMETRY_FIELD_LONGITUDE);
    telemetry_write_field_int32(telem, -10, TELEMETRY_FIELD_ALTITUDE);
    telemetry_write_field_uint32(telem, 359, TELEMETRY_FIELD_HEADING);
    
    
    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    
    /* extract data */
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_COUNT);
    printf("Field count: %d\n", uintdata);
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_DATE);
    printf("Date: %s\n", base40_decode(base40_data, uintdata));
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_TIME);
    printf("Time: %s\n", base40_decode(base40_data, uintdata));
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_MILLISECOND);
    printf("ms: %d\n", uintdata);
    telemetry_read_field_float(telem, &floatdata, TELEMETRY_FIELD_LATITUDE);
    printf("Latitude: %f\n", floatdata);
    telemetry_read_field_float(telem, &floatdata, TELEMETRY_FIELD_LONGITUDE);
    printf("Longitude: %f\n", floatdata);
    telemetry_read_field_int32(telem, &intdata, TELEMETRY_FIELD_ALTITUDE);
    printf("Altitude: %d\n", intdata);
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_HEADING);
    printf("Heading: %d\n", uintdata);
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_MISSION_ID);
    printf("ID: %s\n", base40_decode(base40_data, uintdata));
    telemetry_read_crc32(telem, &uintdata);
    printf("CRC: 0x%x\n", uintdata);
    
    telemetry_delete_packet(telem);
    
    return 0;
    
}
