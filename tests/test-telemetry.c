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
    
    /* print it */
    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    /* insert uint32 */
    telemetry_write_field_uint32(telem, 123, TELEMETRY_FIELD_PACKET_NUMBER);
    
    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    /* packet type */
    telemetry_write_field_uint32(telem, TELEMETRY_TYPE_TELEMETRY, TELEMETRY_FIELD_PACKET_TYPE);

    /* insert float */
    telemetry_write_field_float(telem, 43.31653976658865, TELEMETRY_FIELD_LATITUDE);
    
    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    /* insert int32 */
    telemetry_write_field_int32(telem, -3, TELEMETRY_FIELD_ALTITUDE);
    
    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");

    /* insert base40 data */
    telemetry_write_field_uint32(telem, base40_encode("EKI3"), TELEMETRY_FIELD_MISSION_ID);

    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    
    /* extract data */
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_COUNT);
    printf("Field count: %d\n", uintdata);
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_PACKET_TYPE);
    printf("Packet type: 0x%x\n", uintdata);
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_PACKET_NUMBER);
    printf("Packet number: %d\n", uintdata);
    telemetry_read_field_float(telem, &floatdata, TELEMETRY_FIELD_LATITUDE);
    printf("Latitude: %f\n", floatdata);
    telemetry_read_field_int32(telem, &intdata, TELEMETRY_FIELD_ALTITUDE);
    printf("Altitude: %d\n", intdata);
    telemetry_read_field_uint32(telem, &uintdata, TELEMETRY_FIELD_MISSION_ID);
    printf("ID: %s\n", base40_decode(base40_data, uintdata));
    telemetry_read_crc32(telem, &uintdata);
    printf("CRC: 0x%x\n", uintdata);
    
    /* check data */
    uint8_t test;
    test = telemetry_check_data(telem->data, telem->len);
    if (test == TELEMETRY_OK) {
        printf("Packet data OK\n");
    } else {
        printf("Packet data not valid: %d\n", test);
    }

    telemetry_delete_packet(telem);
    
    return 0;
    
}
