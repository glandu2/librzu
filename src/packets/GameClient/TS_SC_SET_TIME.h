#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SET_TIME_DEF(_) \
	_(simple)(int32_t, gap)

#define TS_SC_SET_TIME_ID(X) \
	X(10, version < EPIC_9_6_3) \
	X(1010, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SET_TIME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SET_TIME_DEF

