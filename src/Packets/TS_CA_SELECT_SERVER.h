#ifndef PACKETS_TS_CA_SELECT_SERVER_H
#define PACKETS_TS_CA_SELECT_SERVER_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_SELECT_SERVER : public TS_MESSAGE
{
	uint16_t server_idx;
	static const uint16_t packetID = 10023;
};
#pragma pack(pop)

#endif // PACKETS_TS_CA_SELECT_SERVER_H
