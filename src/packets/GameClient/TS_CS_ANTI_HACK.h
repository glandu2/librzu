#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ANTI_HACK_DEF(_) \
	_(simple)(uint16_t, nLength) \
	_(array)(uint8_t, byBuffer, 400)

#define TS_CS_ANTI_HACK_ID(X) \
	X(54, version < EPIC_9_6_3) \
	X(1054, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ANTI_HACK, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ANTI_HACK_DEF

