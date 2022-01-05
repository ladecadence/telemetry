#include "telemetry.h"
#include <stdio.h>

telemetry_packet_t* telemetry_create_packet(uint8_t number_fields) 
{
    /* check max number of fields */
    if (number_fields > TELEMETRY_MAX_FIELDS) {
        return NULL;
    }
    
    /* init packet */
    telemetry_packet_t* packet;
    packet = (telemetry_packet_t*) malloc (sizeof (telemetry_packet_t));
    packet->len = number_fields + 3;
    
    /* reserve data (fields + start + crc + end * bytes per field)  */
    uint8_t* data = calloc((packet->len * TELEMETRY_BYTES_PER_FIELD), sizeof(uint8_t));
    
    /* check if allocated  */
    if (!data) {
        return NULL;
    }
    
    packet->data = data;
    
    /* write base data */
    telemetry_write_field_uint32(packet, TELEMETRY_START_MARKER, TELEMETRY_FIELD_START); /* start marker */
    telemetry_write_field_uint32(packet, TELEMETRY_END_MARKER, packet->len-1);           /* end marker */
    telemetry_write_field_uint32 (packet, number_fields, TELEMETRY_FIELD_COUNT);         /* number of fields */
    
    /*  recalculate CRC */
    uint32_t crc = crc32(&packet->data[1*TELEMETRY_BYTES_PER_FIELD], (packet->len-3) * TELEMETRY_BYTES_PER_FIELD);
    crc = htonl(crc); /* endianness */
    for (int i=0; i<TELEMETRY_BYTES_PER_FIELD; i++) {
        packet->data[((packet->len-2) * TELEMETRY_BYTES_PER_FIELD) + i] = crc >> i*8;
    }
    
    return packet;
}



void telemetry_delete_packet(telemetry_packet_t* packet) {
    // free allocated memory
    if (packet != NULL) {
        if (packet->data != NULL) {
            free(packet->data);
        }
        free (packet);
    }
}


uint8_t telemetry_write_field_uint32(telemetry_packet_t* packet, uint32_t data, uint8_t field_number) 
{
    /* check fields */
    if (field_number > packet->len) {
        return TELEMETRY_ERROR_FIELD;
    }
    
    /* use network byte order */
    data =  htonl(data);

    /* write data */
    for (int i = 0; i < TELEMETRY_BYTES_PER_FIELD; i++) {
        packet->data[(field_number * TELEMETRY_BYTES_PER_FIELD) + i] = data >> i*8;
    }
    
    /* recalculate CRC */
    uint32_t crc = crc32(&packet->data[1*TELEMETRY_BYTES_PER_FIELD], (packet->len-3) * TELEMETRY_BYTES_PER_FIELD);
    crc = htonl(crc); /* endianness */
    for (int i = 0; i < TELEMETRY_BYTES_PER_FIELD; i++) {
        packet->data[((packet->len-2) * TELEMETRY_BYTES_PER_FIELD) + i] = crc >> i*8;
    }
    
    return TELEMETRY_OK;
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

uint8_t telemetry_write_raw_data(telemetry_packet_t* packet, uint8_t* data, uint8_t len, uint8_t field_number)
{
    /* check field number, can't overwrite basic fields */
    if (field_number < TELEMETRY_FIELD_VARIABLE) {
        return TELEMETRY_ERROR_FIELD;
    }

    /* check len, must be aligned */
    if (len % TELEMETRY_BYTES_PER_FIELD != 0) {
        return TELEMETRY_ERROR_ALIGN;
    }

    /* and must fit */
    if ((len / TELEMETRY_BYTES_PER_FIELD) > (packet->len - 3 - (field_number-1))) {
        return TELEMETRY_ERROR_LENGTH;
    }

    /* ok, write it */
    for (int i=0; i<len; i++) {
        packet->data[(field_number * TELEMETRY_BYTES_PER_FIELD) + i] = data[i];
    }

    /* recalculate CRC */
    uint32_t crc = crc32(&packet->data[1*TELEMETRY_BYTES_PER_FIELD], (packet->len-3) * TELEMETRY_BYTES_PER_FIELD);
    crc = htonl(crc); /* endianness */
    for (int i = 0; i < TELEMETRY_BYTES_PER_FIELD; i++) {
        packet->data[((packet->len-2) * TELEMETRY_BYTES_PER_FIELD) + i] = crc >> i*8;
    }

    return TELEMETRY_OK;
}

uint32_t telemetry_read_field_uint32(telemetry_packet_t* packet, uint8_t field_number) 
{
    /* check fields */
    if (field_number > packet->len) {
        return 0;
    }
    
    uint32_t data = 0;
    for (int i=TELEMETRY_BYTES_PER_FIELD-1; i>=0; i--) {
        data += packet->data[(field_number * TELEMETRY_BYTES_PER_FIELD) + i] << 8*i;
    }
    
    /* check network byte order */
    return ntohl(data);
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

uint8_t telemetry_read_raw_data(telemetry_packet_t* packet, uint8_t* data, uint8_t len, uint8_t field_number)
{
    /* check field number, basic fields are never raw data*/
    if (field_number < TELEMETRY_FIELD_VARIABLE) {
        return TELEMETRY_ERROR_FIELD;
    }

    /* check len, must be aligned */
    if (len % TELEMETRY_BYTES_PER_FIELD != 0) {
        return TELEMETRY_ERROR_ALIGN;
    }

    /* and must have enough data */
    if ((len / TELEMETRY_BYTES_PER_FIELD) > (packet->len - 3 - (field_number-1))) {
        return TELEMETRY_ERROR_LENGTH;
    }

    /* ok, read it */
    for (int i=0; i<len; i++) {
        data[i] = packet->data[(field_number * TELEMETRY_BYTES_PER_FIELD) + i];
    }

    return TELEMETRY_OK;

}


packet_valid_t telemetry_check_data(uint8_t* data, uint8_t len)
{

    /* check field counter */
    uint32_t fields = 0;
    for (int i=TELEMETRY_BYTES_PER_FIELD-1; i>=0; i--) {
        fields += data[(TELEMETRY_FIELD_COUNT * TELEMETRY_BYTES_PER_FIELD) + i] << 8*i;
    }

    fields = ntohl(fields) + 3;


    if (fields != (len)) {
        return PACKET_BAD_FIELDS;
    }

    /* check CRC */
    uint32_t data_crc = 0;
    for (int i=TELEMETRY_BYTES_PER_FIELD-1; i>=0; i--) {
        data_crc += data[((len-2) * TELEMETRY_BYTES_PER_FIELD) + i] << 8*i;
    }

    data_crc = ntohl(data_crc);

    uint32_t calculated_crc = crc32(data+TELEMETRY_BYTES_PER_FIELD, (len-3) * TELEMETRY_BYTES_PER_FIELD);

    if (data_crc != calculated_crc) {
        return PACKET_BAD_CRC;
    }

    /* nothing bad detected */
    return PACKET_OK;

}
