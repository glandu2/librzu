#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_STATE_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(uint16_t, state_handle) \
	_(def)(simple) (uint16_t, state_level) \
	_(impl)(simple)(uint16_t, state_level, version >= EPIC_9_5_2) \
	_(simple)(uint32_t, state_code) \
	_(impl)(simple)(uint16_t, state_level, version >= EPIC_4_1 && version < EPIC_9_5_2) \
	_(impl)(simple)(int8_t, state_level, version < EPIC_4_1) \
	_(simple)(ar_time_t, end_time) \
	_(simple)(ar_time_t, start_time, version >= EPIC_3) \
	_(simple)(int32_t, state_value, version >= EPIC_4_1) \
	_(string)(state_string_value, 32, version >= EPIC_4_1 && version < EPIC_9_5_2)

#define TS_SC_STATE_ID(X) \
	X(505, version < EPIC_9_6_3) \
	X(1505, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_STATE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_STATE_DEF

