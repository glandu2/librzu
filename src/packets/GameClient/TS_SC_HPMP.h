#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HPMP_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, add_hp) \
	_(simple)(int32_t, hp) \
	_(simple)(int32_t, max_hp) \
	_(def)(simple) (int32_t, add_mp) \
	_(impl)(simple)(int32_t, add_mp, version >= EPIC_4_1) \
	_(impl)(simple)(uint16_t, add_mp, version < EPIC_4_1) \
	_(def)(simple) (int32_t, mp) \
	_(impl)(simple)(int32_t, mp, version >= EPIC_4_1) \
	_(impl)(simple)(uint16_t, mp, version < EPIC_4_1) \
	_(def)(simple) (int32_t, max_mp) \
	_(impl)(simple)(int32_t, max_mp, version >= EPIC_4_1) \
	_(impl)(simple)(uint16_t, max_mp, version < EPIC_4_1) \
	_(simple)(bool, need_to_display, version >= EPIC_4_1)

#define TS_SC_HPMP_ID(X) \
	X(509, version < EPIC_9_6_3) \
	X(1509, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_HPMP, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HPMP_DEF

