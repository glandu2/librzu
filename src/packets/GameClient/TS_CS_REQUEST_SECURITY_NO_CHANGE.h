#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_SECURITY_NO_CHANGE_DEF(_)

#define TS_CS_REQUEST_SECURITY_NO_CHANGE_ID(X) \
	X(9008, version < EPIC_9_6_3) \
	X(8108, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_REQUEST_SECURITY_NO_CHANGE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REQUEST_SECURITY_NO_CHANGE_DEF

