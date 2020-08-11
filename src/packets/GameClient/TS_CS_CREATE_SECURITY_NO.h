#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CREATE_SECURITY_NO_DEF(_) \
	_(array)(uint8_t, password, 19) \
	_(array)(uint8_t, security_no, 19)

#define TS_CS_CREATE_SECURITY_NO_ID(X) \
	X(9006, version < EPIC_9_6_3) \
	X(8106, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CREATE_SECURITY_NO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CREATE_SECURITY_NO_DEF

