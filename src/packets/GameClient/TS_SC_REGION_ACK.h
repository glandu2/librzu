#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_REGION_ACK_DEF(_) \
	_(simple)(int32_t, rx) \
	_(simple)(int32_t, ry)

#define TS_SC_REGION_ACK_ID(X) \
	X(11, version < EPIC_9_6_3) \
	X(1011, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_REGION_ACK, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_REGION_ACK_DEF

