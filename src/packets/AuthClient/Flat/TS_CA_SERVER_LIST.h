#ifndef PACKETS_TS_CA_SERVER_LIST_H
#define PACKETS_TS_CA_SERVER_LIST_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_SERVER_LIST : public TS_MESSAGE
{
	static const uint16_t packetID = 10021;
};

#pragma pack(pop)

#endif // PACKETS_TS_CA_SERVER_LIST_H
