#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_LOGOUT_DEF(_)

#define TS_CS_LOGOUT_ID(X) \
	X(27, version < EPIC_9_6_3) \
	X(1027, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_LOGOUT, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_LOGOUT_DEF

