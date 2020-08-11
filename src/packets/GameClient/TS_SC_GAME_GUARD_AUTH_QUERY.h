#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_GAME_GUARD_AUTH_V1_DEF(_) \
	_(simple)(uint32_t, dwIndex) \
	_(simple)(uint32_t, dwValue1) \
	_(simple)(uint32_t, dwValue2) \
	_(simple)(uint32_t, dwValue3)
CREATE_STRUCT(TS_GAME_GUARD_AUTH_V1);
#undef TS_GAME_GUARD_AUTH_V1_DEF

#define TS_GAME_GUARD_AUTH_V2_DEF(_) \
	_(count)(uint16_t, data) \
	_(simple)(uint16_t, unknown) \
	_(dynarray)(uint8_t, data)
CREATE_STRUCT(TS_GAME_GUARD_AUTH_V2);
#undef TS_GAME_GUARD_AUTH_V2_DEF

#define TS_GAME_GUARD_AUTH_DEF(_) \
	_(simple)(TS_GAME_GUARD_AUTH_V1, authv1, version < EPIC_9_1) \
	_(simple)(TS_GAME_GUARD_AUTH_V2, authv2, version >= EPIC_9_1)
CREATE_STRUCT(TS_GAME_GUARD_AUTH);
#undef TS_GAME_GUARD_AUTH_DEF

#define TS_SC_GAME_GUARD_AUTH_QUERY_DEF(_) \
	_(simple)(TS_GAME_GUARD_AUTH, auth)

#define TS_SC_GAME_GUARD_AUTH_QUERY_ID(X) \
	X(55, version < EPIC_9_6_3) \
	X(1055, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_GAME_GUARD_AUTH_QUERY, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_GAME_GUARD_AUTH_QUERY_DEF

