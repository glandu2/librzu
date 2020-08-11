#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_QUEST_INFO_DEF(_) \
	_(simple)(int32_t, code)

// Since EPIC_6_3
#define TS_CS_QUEST_INFO_ID(X) \
	X(604, version < EPIC_9_6_3) \
	X(1604, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_QUEST_INFO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_QUEST_INFO_DEF

