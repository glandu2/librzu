#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHANGE_SECURITY_NO_DEF(_)

#define TS_SC_CHANGE_SECURITY_NO_ID(X) \
	X(9009, version < EPIC_9_6_3) \
	X(8109, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CHANGE_SECURITY_NO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CHANGE_SECURITY_NO_DEF

