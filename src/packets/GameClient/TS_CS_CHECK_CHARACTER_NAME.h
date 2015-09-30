#ifndef PACKETS_TS_CS_CHECK_CHARACTER_NAME_H
#define PACKETS_TS_CS_CHECK_CHARACTER_NAME_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CS_CHECK_CHARACTER_NAME : public TS_MESSAGE
{
	char name[19];
	static const int packetID = 2006;
};
#pragma pack(pop)

#endif // PACKETS_TS_CS_CHECK_CHARACTER_NAME_H
