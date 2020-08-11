#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_INSTANCE_GAME_SCORE_REQUEST_DEF(_) \
	_(simple)(uint32_t, holicpoint) \
	_(simple)(uint32_t, bearroad_ranking) \
	_(simple)(uint32_t, deathmatch_kill_count) \
	_(simple)(uint32_t, deathmatch_death_count) \
	_(simple)(int32_t, battle_arena_point, version >= EPIC_8_1) \
	_(simple)(int32_t, battle_arena_mvp_count, version >= EPIC_8_1) \
	_(array)(int32_t, battle_arena_record_classic, 2, version >= EPIC_8_1) \
	_(array)(int32_t, battle_arena_record_slaughter, 2, version >= EPIC_8_1) \
	_(array)(int32_t, battle_arena_record_bingo, 2, version >= EPIC_8_1)

// Since EPIC_6_3
#define TS_SC_INSTANCE_GAME_SCORE_REQUEST_ID(X) \
	X(4253, true)

CREATE_PACKET_VER_ID(TS_SC_INSTANCE_GAME_SCORE_REQUEST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_INSTANCE_GAME_SCORE_REQUEST_DEF

