#ifndef PACKETS_TS_SC_MOVE_ACK_H
#define PACKETS_TS_SC_MOVE_ACK_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_MOVE_ACK_DEF(_) \
	_(simple)(ar_time_t, time) \
	_(simple)(int8_t, speed)

#define TS_SC_MOVE_ACK_ID(X) \
	X(6, version < EPIC_9_2) \
	X(66, version >= EPIC_9_2)

CREATE_PACKET_VER_ID(TS_SC_MOVE_ACK);

#endif // PACKETS_TS_SC_MOVE_ACK_H
