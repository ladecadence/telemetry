Simple telemetry format that can be used with low resource platforms like 8-bit
microcontrollers and low speed radio links.

# Data format

Packet data is a byte array with a fixed length of 4 bytes per data field, so 
each field can be used as a 32 bit integer or a single precision float. A type
field can define other packet types and payloads with different internal
structure. To avoid endiannes problems, the library uses network byte order 
(big-endian) to store and transmit data.

For data transmission the data is encoded using COBS (Consistent Overhead
Byte Stuffing) so packet boundaries can be easily identified looking for a 
0x00 byte.

# Fields

LoRa payload is maximum 255 bytes, so 255/4 = 63 Max fields:
payload + CRC = 62 fields (248 bytes) available 
for the payload.

## Mandatory fields

* 0: Field count       - uint32 (excluding CRC)
* 1: Packet number     - uint32
* 2: Packet type       - uint32 (0x77777777 - Telemetry packet)
* 3: Packet flags      - uint32 (see flags section)

... 

* N-1: CRC32           - uint32

## Recommended telemetry packet (Type 0x77777777) 

* 4: Date              - uint32 (Base-40 encoded. YYMMDD)
* 5: Time              - uint32 (Base-40 encoded. HHMMSS)
* 6: Millisecond       - uint32 (ms)
* 7: Mission ID        - uint32 (Base-40 encoded. Up to 6 characters)
* 8: Latitude          - float (decimal)
* 9: Longitude         - float (decimal)
* 10: Altitude         - int32 (meters, shouldn't be negative, but can be)
* 11: Heading          - uint32 (degrees)

## Flags

The flag field currently can indicate:

* Bit 31:              - Multipacket, this packet data is split in several subpackets, check last two bytes
* Bit 30:              - Test packet, data from this packet can be ignored.

...

* Byte 15..8:          - Multipacket subpacket count 
* Byte 7..0:           - Multipacket subpacket number (subpacket number from count)

