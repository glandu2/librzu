#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_LOGOUT_DEF(_)

#define TS_CS_REQUEST_LOGOUT_ID(X) \
	X(26, version < EPIC_9_6_3) \
	X(1026, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_REQUEST_LOGOUT, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REQUEST_LOGOUT_DEF

