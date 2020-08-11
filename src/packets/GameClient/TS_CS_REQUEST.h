#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_DEF(_) \
	_(simple)(uint8_t, t) \
	_(endstring)(command, true)

#define TS_CS_REQUEST_ID(X) \
	X(60, version < EPIC_9_6_3) \
	X(1060, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_REQUEST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REQUEST_DEF

