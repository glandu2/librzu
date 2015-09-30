#ifndef PACKETS_TS_CS_DELETE_CHARACTER_H
#define PACKETS_TS_CS_DELETE_CHARACTER_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CS_DELETE_CHARACTER : public TS_MESSAGE
{
	char name[19];
	static const int packetID = 2003;
};
#pragma pack(pop)

#endif // PACKETS_TS_CS_DELETE_CHARACTER_H
