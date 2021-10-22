#include <stdio.h>
#include "../src/base40.h"

void main(void) {
    uint32_t id;
    char* original_id = "EKI3";
    char cid[7];

    id = encode_mission_id(original_id);
    printf("Encoded: %d\n", id);

    decode_mission_id(cid, id);
    printf("Decoded: %s\n", cid);

}
