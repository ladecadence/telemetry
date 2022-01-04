Simple telemetry format that can be used with low resource platforms like 8-bit
microcontrollers and low speed radio links.

# Data format

Packet data is a byte array with a fixed length of 4 bytes per data field, so 
each field can be used as a 32bit integer or a single precision float. A type
field can define other packet types and payloads with different internal
structure. To avoid endiannes problems, the library uses network byte order 
(big-endian) to store and transmit data.

# Fields

LoRa payload is maximum 255 bytes, so 255/4 = 63 Max fields:
Start marker + payload + CRC + End marker = 60 fields (240b bytes) available 
for the payload.

## Mandatory fields

* 0: Start Marker      - 0xAA 0x55 0xAA 0x55
* 1: Field count       - uint32 (excluding start and end markers and CRC)
* 2: Packet number     - uint32
* 3: Packet type       - uint32 (0x77777777 - Telemetry packet)
... 

* N-2: CRC32             - Int32 (excludes start and end markers)
* N-1: End Marker        - 0xEE 0xDD 0xEE 0xDD

## Recommended telemetry packet (Type 0x77777777) 

* 4: Date              - uint32 (Base-40 encoded. YYMMDD)
* 5: Time              - uint32 (Base-40 encoded. HHMMSS)
* 6: Millisecond       - uint32 (ms)
* 7: Mission ID        - uint32 (Base-40 encoded. Up to 6 characters)
* 8: Latitude          - float (decimal)
* 9: Longitude         - float (decimal)
* 10: Altitude         - int32 (meters, shouldn't be negative, but can be)
* 11: Heading          - uint32 (degrees)



