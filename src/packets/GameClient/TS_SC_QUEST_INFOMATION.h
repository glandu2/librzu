#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_QUEST_PROGRESS : int32_t
{
	IS_STARTABLE = 0,
	IS_IN_PROGRESS = 1,
	IS_FINISHABLE = 2,
};

// Seems unused
#define TS_SC_QUEST_INFOMATION_DEF(_) \
	_(simple)(int32_t, code) \
	_(simple)(TS_QUEST_PROGRESS, nProgress) \
	_(simple)(uint16_t, trigger_length)

#define TS_SC_QUEST_INFOMATION_ID(X) \
	X(602, version < EPIC_9_6_3) \
	X(1602, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_QUEST_INFOMATION, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_QUEST_INFOMATION_DEF

