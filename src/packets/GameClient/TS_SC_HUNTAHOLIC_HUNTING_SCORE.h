#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HUNTAHOLIC_HUNTING_SCORE_DEF(_) \
	_(simple)(int32_t, huntaholic_id) \
	_(simple)(int32_t, personal_kill_count) \
	_(simple)(int32_t, personal_score) \
	_(simple)(int32_t, kill_count) \
	_(simple)(int32_t, score) \
	_(simple)(double, point_advantage) \
	_(simple)(double, point_rate) \
	_(simple)(int32_t, gain_point) \
	_(simple)(int8_t, result_type)

#define TS_SC_HUNTAHOLIC_HUNTING_SCORE_ID(X) \
	X(4006, true)

CREATE_PACKET_VER_ID(TS_SC_HUNTAHOLIC_HUNTING_SCORE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HUNTAHOLIC_HUNTING_SCORE_DEF

