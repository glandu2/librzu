#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SET_SUB_TITLE_DEF(_) \
	_(simple)(int32_t, index) \
	_(simple)(int32_t, code)

// Since EPIC_8_1
#define TS_CS_SET_SUB_TITLE_ID(X) \
	X(630, version < EPIC_9_6_3) \
	X(1630, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SET_SUB_TITLE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SET_SUB_TITLE_DEF

