#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_CLEAR_SECURITY_NO_DEF(_)

#define TS_CS_REQUEST_CLEAR_SECURITY_NO_ID(X) \
	X(9010, version < EPIC_9_6_3) \
	X(8110, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_REQUEST_CLEAR_SECURITY_NO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REQUEST_CLEAR_SECURITY_NO_DEF

