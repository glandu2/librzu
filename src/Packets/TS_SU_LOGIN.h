#ifndef TS_SU_LOGIN_H
#define TS_SU_LOGIN_H

#include "Packets/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_SU_LOGIN : public TS_MESSAGE
{
	char server_name[21];

	static const uint16_t packetID = 50001;
};
#pragma pack(pop)

#endif // TS_SU_LOGIN_H
