#include "telemetry.h"
#include <stdio.h>

telemetry_packet_t* telemetry_create_packet(uint8_t number_fields) 
{
    /* check max number of fields */
    if (number_fields > MAX_TELEMETRY_FIELDS) {
        return NULL;
    }
    
    /* init packet */
    telemetry_packet_t* packet;
    packet = (telemetry_packet_t*) malloc (sizeof (telemetry_packet_t));
    packet->len = number_fields + 3;
    
    /* reserve data (fields + start + crc + end * bytes per field)  */
    uint8_t* data = calloc((packet->len * BYTES_PER_FIELD), sizeof(uint8_t));
    
    /* check if allocated  */
    if (!data) {
        return NULL;
    }
    
    packet->data = data;
    
    /* write base data */
    telemetry_write_field_uint32(packet, START_MARKER, 0);
    telemetry_write_field_uint32(packet, END_MARKER, packet->len-1);
    
    /*  recalculate CRC */
    uint32_t crc = crc32(&packet->data[1*BYTES_PER_FIELD], (packet->len-3) * BYTES_PER_FIELD);
    for (int i=0; i<BYTES_PER_FIELD; i++) {
        packet->data[((packet->len-2) * BYTES_PER_FIELD) + i] = crc >> i*8;
    }
    
    return packet;
}



uint8_t telemetry_write_field_uint32(telemetry_packet_t* packet, uint32_t data, uint8_t field_number) 
{
    /* check fields */
    if (field_number > packet->len) {
        return EXIT_FAILURE;
    }
    
    /* write data */
    for (int i=0; i<BYTES_PER_FIELD; i++) {
        packet->data[(field_number * BYTES_PER_FIELD) + i] = data >> i*8;
    }
    
    /*  recalculate CRC */
    uint32_t crc = crc32(&packet->data[1*BYTES_PER_FIELD], (packet->len-3) * BYTES_PER_FIELD);
    for (int i=0; i<BYTES_PER_FIELD; i++) {
        packet->data[((packet->len-2) * BYTES_PER_FIELD) + i] = crc >> i*8;
    }
    
    return EXIT_SUCCESS;                                                                                                
}



uint8_t telemetry_write_field_int32(telemetry_packet_t* packet, int32_t data, uint8_t field_number)
{
    /* just cast it */
    return telemetry_write_field_uint32(packet, (uint32_t) data, field_number); 
}



uint8_t telemetry_write_field_float(telemetry_packet_t* packet, float data, uint8_t field_number)
{
    
    /* write data */
    uint32_t intdata;
    memcpy(&intdata, &data, sizeof(data));
    
    return telemetry_write_field_uint32(packet, intdata, field_number);
}



uint32_t telemetry_read_field_uint32(telemetry_packet_t* packet, uint8_t field_number) 
{
    /* check fields */
    if (field_number > packet->len) {
        return 0;
    }
    
    uint32_t data = 0;
    for (int i=BYTES_PER_FIELD-1; i>=0; i--) {
        data += packet->data[(field_number * BYTES_PER_FIELD) + i] << 8*i;
    }
    
    return data;
}


int32_t telemetry_read_field_int32(telemetry_packet_t* packet, uint8_t field_number) 
{
    return (int32_t) telemetry_read_field_uint32(packet, field_number);
}



uint32_t telemetry_read_crc32(telemetry_packet_t* packet) 
{   
    return telemetry_read_field_uint32(packet, packet->len-2);
}



float telemetry_read_field_float(telemetry_packet_t* packet, uint8_t field_number) 
{
    /* check fields */
    if (field_number > packet->len) {
        return 0.0;
    }
    
    uint32_t intdata = telemetry_read_field_uint32(packet, field_number);
    float data;
    memcpy(&data, &intdata, sizeof(data));
    return data;
}
