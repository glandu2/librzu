#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_TURN_ON_PK_MODE_DEF(_)

#define TS_CS_TURN_ON_PK_MODE_ID(X) \
	X(800, version < EPIC_9_6_3) \
	X(1800, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_TURN_ON_PK_MODE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_TURN_ON_PK_MODE_DEF

