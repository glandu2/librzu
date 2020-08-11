#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TARGETING_DEF(_) \
	_(simple)(ar_handle_t, target)

#define TS_CS_TARGETING_ID(X) \
	X(511, version < EPIC_9_6_3) \
	X(1511, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_TARGETING, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_TARGETING_DEF

