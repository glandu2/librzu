#ifndef PACKETS_TS_SC_INSTANCE_GAME_SCORE_REQUEST_H
#define PACKETS_TS_SC_INSTANCE_GAME_SCORE_REQUEST_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_INSTANCE_GAME_SCORE_REQUEST_DEF(_) \
	_(simple)(uint32_t, holicpoint) \
	_(simple)(uint32_t, bearroad_ranking) \
	_(simple)(uint32_t, deathmatch_kill_count) \
	_(simple)(uint32_t, deathmatch_death_count) \
	_(simple)(int32_t, battle_arena_point) \
	_(simple)(int32_t, battle_arena_mvp_count) \
	_(array)(int32_t, battle_arena_record_classic, 2) \
	_(array)(int32_t, battle_arena_record_slaughter, 2) \
	_(array)(int32_t, battle_arena_record_bingo, 2)

CREATE_PACKET(TS_SC_INSTANCE_GAME_SCORE_REQUEST, 4253);

#endif // PACKETS_TS_SC_INSTANCE_GAME_SCORE_REQUEST_H
