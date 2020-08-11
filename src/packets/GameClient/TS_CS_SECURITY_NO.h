#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SECURITY_NO_DEF(_) \
	_(simple)(int32_t, mode) \
	_(string)(security_no, 19)

#define TS_CS_SECURITY_NO_ID(X) \
	X(9005, version < EPIC_9_6_3) \
	X(8105, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SECURITY_NO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SECURITY_NO_DEF

