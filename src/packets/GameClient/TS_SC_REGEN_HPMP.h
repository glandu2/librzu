#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_REGEN_HPMP_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(def)(simple)(int32_t, hp_regen) \
	_(impl)(simple)(int32_t, hp_regen, version >= EPIC_7_3) \
	_(impl)(simple)(uint16_t, hp_regen, version < EPIC_7_3) \
	_(def)(simple)(int32_t, mp_regen) \
	_(impl)(simple)(int32_t, mp_regen, version >= EPIC_7_3) \
	_(impl)(simple)(uint16_t, mp_regen, version < EPIC_7_3) \
	_(simple)(int32_t, hp) \
	_(def)(simple)(int32_t, mp) \
	_(impl)(simple)(int32_t, mp, version >= EPIC_7_3) \
	_(impl)(simple)(uint16_t, mp, version < EPIC_7_3) \

#define TS_SC_REGEN_HPMP_ID(X) \
	X(516, version < EPIC_9_6_3) \
	X(1516, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_REGEN_HPMP, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_REGEN_HPMP_DEF

