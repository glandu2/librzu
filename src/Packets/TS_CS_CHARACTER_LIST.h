#ifndef PACKETS_TS_CS_CHARACTER_LIST_H
#define PACKETS_TS_CS_CHARACTER_LIST_H

#include "Packets/PacketBaseMessage.h"
#include "Packets/PacketEnums.h"

#pragma pack(push, 1)
struct TS_CS_CHARACTER_LIST : public TS_MESSAGE
{
	char account[61];

	static const int packetID = 2001;
};
#pragma pack(pop)

#endif // PACKETS_TS_CS_CHARACTER_LIST_H
