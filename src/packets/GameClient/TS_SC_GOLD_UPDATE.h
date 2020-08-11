#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_GOLD_UPDATE_DEF(_) \
	_(def)(simple) (uint64_t, gold) \
	_(impl)(simple)(uint64_t, gold, version >= EPIC_4_1_1) \
	_(impl)(simple)(uint32_t, gold, version < EPIC_4_1_1) \
	_(simple) (uint32_t, chaos, version > EPIC_4_1_1)

#define TS_SC_GOLD_UPDATE_ID(X) \
	X(1001, version < EPIC_9_6_3) \
	X(2001, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_GOLD_UPDATE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_GOLD_UPDATE_DEF

