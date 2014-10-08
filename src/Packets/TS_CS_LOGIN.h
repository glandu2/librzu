#ifndef TS_CS_LOGIN_H
#define TS_CS_LOGIN_H

#include "Packets/PacketBaseMessage.h"
#include "Packets/PacketEnums.h"

#pragma pack(push, 1)
struct TS_CS_LOGIN : public TS_MESSAGE
{
	char szName[19];
	char race;
	static const int packetID = 1;
};
#pragma pack(pop)

#endif // TS_CS_LOGIN_H
