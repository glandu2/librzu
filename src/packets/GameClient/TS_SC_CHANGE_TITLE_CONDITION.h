#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHANGE_TITLE_CONDITION_DEF(_) \
	_(simple)(int32_t, condition_id) \
	_(simple)(int64_t, count)

// Since EPIC_8_1
#define TS_SC_CHANGE_TITLE_CONDITION_ID(X) \
	X(636, version < EPIC_9_6_3) \
	X(1636, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CHANGE_TITLE_CONDITION, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CHANGE_TITLE_CONDITION_DEF

