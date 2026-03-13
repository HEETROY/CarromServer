#pragma once
#include "packet.hpp"

struct StrikePacket
{
    PacketHeader header;
    float angle;
    float power;
};

//Memory layout
//
//The packet will look like this:
//
//size   = 2 bytes
//type   = 1 byte
//angle  = 4 bytes
//power  = 4 bytes
//
//Total:
//
//11 bytes
//
//So the actual binary packet is:
//
//[11][2][angle][power]
//
//Where:
//
//11 = packet size
//2  = strike packet
