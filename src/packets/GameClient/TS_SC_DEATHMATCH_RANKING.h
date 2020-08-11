#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_DEATHMATCH_RANKING_INFO_DEF(_) \
	_(simple)(int32_t, rank) \
	_(simple)(uint16_t, kills) \
	_(simple)(uint16_t, deaths) \
	_(simple)(uint32_t, score) \
	_(simple)(uint32_t, unknown_2) \
	_(string)(name, 16)

CREATE_STRUCT(TS_DEATHMATCH_RANKING_INFO);
#undef TS_DEATHMATCH_RANKING_INFO_DEF

#define TS_SC_DEATHMATCH_RANKING_DEF(_) \
	_(simple)(int32_t, time_epoch) \
	_(array)(TS_DEATHMATCH_RANKING_INFO, players, 30)

#define TS_SC_DEATHMATCH_RANKING_ID(X) \
	X(4301, true)

CREATE_PACKET_VER_ID(TS_SC_DEATHMATCH_RANKING, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DEATHMATCH_RANKING_DEF

