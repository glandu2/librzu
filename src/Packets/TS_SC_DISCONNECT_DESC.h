#ifndef PACKETS_TS_SC_DISCONNECT_DESC_H
#define PACKETS_TS_SC_DISCONNECT_DESC_H

#include "Packets/PacketBaseMessage.h"
#include "Packets/PacketEnums.h"

#pragma pack(push, 1)
struct TS_SC_DISCONNECT_DESC : public TS_MESSAGE
{
	char desc_id;

	static const int packetID = 28;
};
#pragma pack(pop)

#endif // PACKETS_TS_SC_DISCONNECT_DESC_H
