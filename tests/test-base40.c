#include <stdio.h>
#include "../src/base40.h"

void main(void) {
    uint32_t id;
    char* original_id = "EKI3";
    char cid[7];

    id = base40_encode(original_id);
    printf("Encoded: %d\n", id);

    base40_decode(cid, id);
    printf("Decoded: %s\n", cid);

}
