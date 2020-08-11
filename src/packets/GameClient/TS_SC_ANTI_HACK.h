#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ANTI_HACK_DEF(_) \
	_(simple)(uint16_t, nLength) \
	_(array)(uint8_t, byBuffer, 400)

#define TS_SC_ANTI_HACK_ID(X) \
	X(53, version < EPIC_9_6_3) \
	X(1053, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ANTI_HACK, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ANTI_HACK_DEF

