#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_STOP_BOOTH_DEF(_)

#define TS_CS_STOP_BOOTH_ID(X) \
	X(701, version < EPIC_9_6_3) \
	X(1701, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_STOP_BOOTH, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_STOP_BOOTH_DEF

