#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SET_SUB_TITLE_DEF(_) \
	_(simple)(int32_t, index) \
	_(simple)(int32_t, code)

// Since EPIC_8_1
#define TS_SC_SET_SUB_TITLE_ID(X) \
	X(631, version < EPIC_9_6_3) \
	X(1631, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SET_SUB_TITLE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SET_SUB_TITLE_DEF

