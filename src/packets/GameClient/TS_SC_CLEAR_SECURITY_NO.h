#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CLEAR_SECURITY_NO_DEF(_) \
	_(simple)(bool, clearable) \

#define TS_SC_CLEAR_SECURITY_NO_ID(X) \
	X(9011, version < EPIC_9_6_3) \
	X(8111, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CLEAR_SECURITY_NO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CLEAR_SECURITY_NO_DEF

