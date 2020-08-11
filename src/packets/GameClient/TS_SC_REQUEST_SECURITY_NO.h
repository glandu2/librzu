#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_REQUEST_SECURITY_NO_DEF(_) \
	_(simple)(int32_t, mode)

#define TS_SC_REQUEST_SECURITY_NO_ID(X) \
	X(9004, version < EPIC_9_6_3) \
	X(8104, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_REQUEST_SECURITY_NO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_REQUEST_SECURITY_NO_DEF

