#pragma once

#include "Packet/PacketDeclaration.h"

// Seems unused
#define TS_SC_REGEN_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, hp_regen_percentage) \
	_(def)(simple)(int32_t, hp_regen_point) \
	_(impl)(simple)(int32_t, hp_regen_point, version >= EPIC_7_3) \
	_(impl)(simple)(int16_t, hp_regen_point, version < EPIC_7_3) \
	_(simple)(int16_t, mp_regen_percentage) \
	_(def)(simple)(int32_t, mp_regen_point) \
	_(impl)(simple)(int32_t, mp_regen_point, version >= EPIC_7_3) \
	_(impl)(simple)(int16_t, mp_regen_point, version < EPIC_7_3) \

#define TS_SC_REGEN_INFO_ID(X) \
	X(510, version < EPIC_9_6_3) \
	X(1510, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_REGEN_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_REGEN_INFO_DEF

