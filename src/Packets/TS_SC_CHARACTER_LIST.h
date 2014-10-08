#ifndef TS_SC_CHARACTER_LIST_H
#define TS_SC_CHARACTER_LIST_H

#include "Packets/PacketBaseMessage.h"
#include "Packets/PacketEnums.h"

#pragma pack(push, 1)
struct TS_SC_CHARACTER_LIST : public TS_MESSAGE_WNA
{
	unsigned int current_server_time;
	unsigned short last_login_index;
	unsigned short count;
	static const int packetID = 2004;
};
#pragma pack(pop)

#endif // TS_SC_CHARACTER_LIST_H
