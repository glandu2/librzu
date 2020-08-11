#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_QUEST_STATUS_DEF(_) \
	_(simple)(int32_t, code) \
	_(def)(array) (uint32_t, status, 6) \
	_(impl)(array)(uint32_t, status, 6, version >= EPIC_6_1) \
	_(impl)(array)(uint32_t, status, 3, version < EPIC_6_1) \
	_(simple)(int8_t, nProgress, version >= EPIC_6_3) \
	_(simple)(ar_time_t, nTimeLimit, version >= EPIC_6_3)

#define TS_SC_QUEST_STATUS_ID(X) \
	X(601, version < EPIC_9_6_3) \
	X(1601, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_QUEST_STATUS, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_QUEST_STATUS_DEF

