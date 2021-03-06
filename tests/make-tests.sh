#!/bin/bash
gcc -g -o test-base40 test-base40.c ../src/base40.c
gcc -g -o test-crc32 test-crc32.c ../src/crc32.c
gcc -g -o test-telemetry test-telemetry.c ../src/telemetry.c ../src/crc32.c ../src/base40.c
gcc -g -o test-rawdata test-rawdata.c ../src/telemetry.c ../src/crc32.c ../src/base40.c
gcc -g -o test-all test-all.c ../src/telemetry.c ../src/crc32.c ../src/base40.c
