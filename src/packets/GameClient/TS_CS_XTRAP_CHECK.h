#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_XTRAP_CHECK_DEF(_) \
	_(array)(uint8_t, pCheckBuffer, 128)

#define TS_CS_XTRAP_CHECK_ID(X) \
	X(59, version < EPIC_9_6_3) \
	X(1059, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_XTRAP_CHECK, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_XTRAP_CHECK_DEF

