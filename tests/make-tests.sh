#!/bin/bash
gcc -o test-base40 test-base40.c ../src/base40.c
gcc -o test-crc32 test-crc32.c ../src/crc32.c
gcc -o test-telemetry test-telemetry.c ../src/telemetry.c ../src/crc32.c ../src/base40.c
gcc -o test-rawdata test-rawdata.c ../src/telemetry.c ../src/crc32.c ../src/base40.c
gcc -o test-all test-all.c ../src/telemetry.c ../src/crc32.c ../src/base40.c
