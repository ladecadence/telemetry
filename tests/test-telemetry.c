#include <stdio.h>
#include "../src/telemetry.h"

int main(void) {
    telemetry_packet_t* telem;
    
    /* create packet */
    telem = telemetry_create_packet(5);
    
    /* print it */
    for (int i=0; i < telem->len*BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    /* insert uint32 */
    telemetry_write_field_uint32(telem, 0x11223344, 2);
    
    for (int i=0; i < telem->len*BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    /* insert float */
    telemetry_write_field_float(telem, 123.456, 3);
    
    for (int i=0; i < telem->len*BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    /* insert int32 */
    telemetry_write_field_int32(telem, -998877, 4);
    
    for (int i=0; i < telem->len*BYTES_PER_FIELD; i++) {
        printf("0x%.2x ", telem->data[i]);
    }
    printf("\n");
    
    /* extract data */
    printf("0x%x\n", telemetry_read_field_uint32(telem, 2));
    printf("%f\n", telemetry_read_field_float(telem, 3));
    printf("%d\n", telemetry_read_field_int32(telem, 4));
    printf("0x%x\n", telemetry_read_crc32(telem));
    
    return 0;
    
}
