#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_GET_CHAOS_DEF(_) \
	_(simple)(ar_handle_t, hPlayer) \
	_(simple)(ar_handle_t, hCorpse) \
	_(simple)(int32_t, nChaos) \
	_(def)(simple)(int32_t, nBonusType) \
	  _(impl)(simple)(int8_t, nBonusType, version >= EPIC_4_1 && version < EPIC_9_6_2) \
	  _(impl)(simple)(int32_t, nBonusType, version >= EPIC_9_6_2) \
	_(def)(simple)(int32_t, nBonusPercent) \
	  _(impl)(simple)(int8_t, nBonusPercent, version >= EPIC_4_1 && version < EPIC_9_6_2) \
	  _(impl)(simple)(int32_t, nBonusPercent, version >= EPIC_9_6_2) \
	_(simple)(int32_t, nBonus, version >= EPIC_4_1)

#define TS_SC_GET_CHAOS_ID(X) \
	X(213, version < EPIC_9_6_3) \
	X(1213, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_GET_CHAOS, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_GET_CHAOS_DEF

