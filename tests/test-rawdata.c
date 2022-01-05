#include <stdio.h>
#include "../src/telemetry.h"

int main(void) {
    telemetry_packet_t* telem;
    char base40_data[7];
    uint8_t raw_data[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
    };
    uint8_t read_data[24];
    uint32_t uintdata;

    /* create packet */
    telem = telemetry_create_packet(9);

    /* print it */
    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");

    /* insert basic info */
    telemetry_write_field_uint32(telem, 123, TELEMETRY_FIELD_PACKET_NUMBER);

    for (int i=0; i < telem->len*TELEMETRY_BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");

    /* packet type */
    telemetry_write_field_uint32(telem, 0xdddddddd, TELEMETRY_FIELD_PACKET_TYPE);

    /* insert raw data */
    uint8_t status = telemetry_write_raw_data(telem, raw_data, 24, TELEMETRY_FIELD_VARIABLE);

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


    /* check data */
    uint8_t test;
    test = telemetry_check_data(telem->data, telem->len);
    if (test == TELEMETRY_OK) {
        printf("Packet data OK\n");
    } else {
        printf("Packet data not valid: %d\n", test);
    }

    /* get raw data */
    printf("Raw data: ");
    status = telemetry_read_raw_data(telem, read_data, 24, TELEMETRY_FIELD_VARIABLE);
    for (int i=0; i < 24; i++) {
        printf("0x%.2x ", read_data[i]);
    }
    printf("\n");

    telemetry_delete_packet(telem);

    return 0;

}
