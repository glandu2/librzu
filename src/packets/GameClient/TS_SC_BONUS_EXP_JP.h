#pragma once

#include "Packet/PacketDeclaration.h"

// Tested on e4 072007
#define TS_BONUS_INFO_DEF(_) \
	_(simple)(int32_t, type) \
	_(simple)(int32_t, rate, version >= EPIC_5_1) \
	_(def)(simple) (int64_t, exp) \
	_(impl)(simple)(int64_t, exp, version >= EPIC_6_1) \
	_(impl)(simple)(int32_t, exp, version < EPIC_6_1) \
	_(simple)(int32_t, jp)

CREATE_STRUCT(TS_BONUS_INFO);
#undef TS_BONUS_INFO_DEF

#define TS_SC_BONUS_EXP_JP_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(count)(uint16_t, bonus) \
	_(dynarray)(TS_BONUS_INFO, bonus)

#define TS_SC_BONUS_EXP_JP_ID(X) \
	X(1004, version < EPIC_9_6_3) \
	X(2004, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_BONUS_EXP_JP, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_BONUS_EXP_JP_DEF

