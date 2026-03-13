#pragma once

enum PacketType
{
    PACKET_JOIN = 1,
    PACKET_STRIKE = 2
};

//Explanation
//
//enum means:
//
//Give names to numbers
//
//So instead of writing:
//
//if(type == 1)
//
//we write:
//
//if(type == PACKET_JOIN)
//
//Much clearer.
