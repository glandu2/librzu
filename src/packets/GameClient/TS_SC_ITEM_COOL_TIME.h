#ifndef PACKETS_TS_SC_ITEM_COOL_TIME_H
#define PACKETS_TS_SC_ITEM_COOL_TIME_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_ITEM_COOL_TIME_DEF(_) \
	_(array)(uint32_t, cool_time, 40)

CREATE_PACKET(TS_SC_ITEM_COOL_TIME, 217);

#endif // PACKETS_TS_SC_ITEM_COOL_TIME_H
