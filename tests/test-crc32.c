#include <stdio.h>
#include <string.h>
#include "../src/crc32.h"

void main() {
    char* data = "123456789";
    
    printf ("CRC32b: 0x%x\n", crc32(data, strlen(data)));
}
