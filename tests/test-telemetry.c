#include <stdio.h>
#include "../src/telemetry.h"

int main(void) {
    telemetry_packet_t* telem;
    char base40_data[7];
    
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
    telemetry_write_field_uint32(telem,  base40_encode("EKI3"), TELEMETRY_FIELD_MISSION_ID);

    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    
    /* extract data */
    printf("Field count: %d\n", telemetry_read_field_uint32(telem, TELEMETRY_FIELD_COUNT));
    printf("Packet type: 0x%x\n", telemetry_read_field_uint32(telem, TELEMETRY_FIELD_PACKET_TYPE));
    printf("Packet number: %d\n", telemetry_read_field_uint32(telem, TELEMETRY_FIELD_PACKET_NUMBER));
    printf("Latitude: %f\n", telemetry_read_field_float(telem, TELEMETRY_FIELD_LATITUDE));
    printf("Altitude: %d\n", telemetry_read_field_int32(telem, TELEMETRY_FIELD_ALTITUDE));
    printf("ID: %s\n", base40_decode(base40_data, telemetry_read_field_uint32(telem, TELEMETRY_FIELD_MISSION_ID)));
    printf("CRC: 0x%x\n", telemetry_read_crc32(telem));
    
    /* check data */
    packet_valid_t test;
    test = telemetry_check_data(telem->data, telem->len);
    if (test == PACKET_OK) {
        printf("Packet data OK\n");
    } else {
        printf("Packet data not valid: %d\n", test);
    }

    telemetry_delete_packet(telem);
    
    return 0;
    
}
