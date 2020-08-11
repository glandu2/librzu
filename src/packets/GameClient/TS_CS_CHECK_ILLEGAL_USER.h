#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHECK_ILLEGAL_USER_DEF(_) \
	_(simple)(uint32_t, log_code)

#define TS_CS_CHECK_ILLEGAL_USER_ID(X) \
	X(57, version < EPIC_9_6_3) \
	X(1057, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CHECK_ILLEGAL_USER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CHECK_ILLEGAL_USER_DEF

