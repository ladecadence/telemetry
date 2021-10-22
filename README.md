# Data format

Packet data is a byte array with a fixed length of 4 bytes per data field, so 
each field can be used as a 32bit integer or a single precision float.

# Fields

LoRa payload is maximum 255 bytes, so 255/4 = 63 Max fields:
Start marker + payload + CRC + End marker = 60 fields available for the payload.

## Mandatory fields

* Start Marker      - 0xAA 0x55 0xAA 0x55
* Field count       - uint32 (excluding start and end markers and CRC)
* Packet number     - uint32
* Date              - uint32 (Base-40 encoded. YYMMDD)
* Time              - uint32 (Base-40 encoded. HHMMSS)
* Millisecond       - uint32 (ms)
* Mission ID        - uint32 (Base-40 encoded. Up to 6 characters)
* Latitude          - float (decimal)
* Longitude         - float (decimal)
* Altitude          - int32 (meters, shouldn't be negative, but can be)
* Heading           - uint32 (degrees)


... 

* CRC32             - Int32 (excludes start and end markers)
* End Marker        - 0xEE 0xDD 0xEE 0xDD



