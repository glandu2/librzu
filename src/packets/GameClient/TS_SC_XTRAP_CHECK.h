#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_XTRAP_CHECK_DEF(_) \
	_(array)(uint8_t, pCheckBuffer, 128)

#define TS_SC_XTRAP_CHECK_ID(X) \
	X(58, version < EPIC_9_6_3) \
	X(1058, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_XTRAP_CHECK, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_XTRAP_CHECK_DEF

