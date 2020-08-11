#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ACHIEVE_TITLE_DEF(_) \
	_(simple)(int32_t, code)

// Since EPIC_8_1
#define TS_SC_ACHIEVE_TITLE_ID(X) \
	X(634, version < EPIC_9_6_3) \
	X(1634, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ACHIEVE_TITLE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ACHIEVE_TITLE_DEF

