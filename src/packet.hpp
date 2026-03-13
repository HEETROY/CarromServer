#pragma once
#include <cstdint>

struct PacketHeader
{
    uint16_t size;
    uint8_t type;
};


//Explanation like you're 5
//
//struct
//
//A struct is a container that groups data together.
//
//Example:
//
//Box
// ├ size
// └ type
//uint16_t size
//unsigned 16-bit integer
//
//Meaning:
//
//2 bytes
//
//Range:
//
//0 → 65535
//
//So our packet can be max 65KB.
//
//uint8_t type
//1 byte number
//
//Range:
//
//0 → 255
//
//This tells the server what the packet is.
