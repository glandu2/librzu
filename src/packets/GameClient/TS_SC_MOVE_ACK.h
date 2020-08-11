#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_MOVE_ACK_DEF(_) \
	_(simple)(ar_time_t, time) \
	_(simple)(int8_t, speed)

#define TS_SC_MOVE_ACK_ID(X) \
	X(6, version < EPIC_9_2) \
	X(66, version >= EPIC_9_2 && version < EPIC_9_6_3) \
	X(1066, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_MOVE_ACK, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_MOVE_ACK_DEF

