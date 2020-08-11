#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_END_QUEST_DEF(_) \
	_(simple)(int32_t, code) \
	_(simple)(int8_t, nOptionalReward)

// Since EPIC_7_3
#define TS_CS_END_QUEST_ID(X) \
	X(605, version < EPIC_9_6_3) \
	X(1605, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_END_QUEST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_END_QUEST_DEF

