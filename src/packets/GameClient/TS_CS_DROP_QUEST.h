#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_DROP_QUEST_DEF(_) \
	_(simple)(int32_t, code)

#define TS_CS_DROP_QUEST_ID(X) \
	X(603, version < EPIC_9_6_3) \
	X(1603, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_DROP_QUEST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_DROP_QUEST_DEF

