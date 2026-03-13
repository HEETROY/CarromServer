#pragma once
#include "packet.hpp"

struct JoinPacket
{
    PacketHeader header;
};

//This packet has no extra data yet.
//
//Meaning the player simply says:
//
//Hello server, I joined.
