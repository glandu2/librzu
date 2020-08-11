#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHECK_BOOTH_STARTABLE_DEF(_)

#define TS_CS_CHECK_BOOTH_STARTABLE_ID(X) \
	X(711, version < EPIC_9_6_3) \
	X(1711, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CHECK_BOOTH_STARTABLE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CHECK_BOOTH_STARTABLE_DEF

