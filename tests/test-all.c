#include <stdio.h>
#include "../src/telemetry.h"

int main(void) {
    telemetry_packet_t* telem;
    char base40_data[7];
    
    /* create packet */
    telem = telemetry_create_packet(TELEMETRY_STANDARD_MIN_FIELDS);
    
    
    /* insert data */
    telemetry_write_field_uint32(telem, 10, TELEMETRY_FIELD_COUNT);
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
    printf("Field count: %d\n", telemetry_read_field_uint32(telem, TELEMETRY_FIELD_COUNT));
    printf("%f\n", telemetry_read_field_float(telem, TELEMETRY_FIELD_LATITUDE));
    printf("%d\n", telemetry_read_field_int32(telem, TELEMETRY_FIELD_ALTITUDE));
    printf("%s\n", base40_decode(base40_data, telemetry_read_field_uint32(telem, TELEMETRY_FIELD_MISSION_ID)));
    printf("0x%x\n", telemetry_read_crc32(telem));
    
    telemetry_delete_packet(telem);
    
    return 0;
    
}
