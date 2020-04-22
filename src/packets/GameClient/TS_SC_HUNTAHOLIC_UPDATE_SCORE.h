#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HUNTAHOLIC_UPDATE_SCORE_DEF(_) \
	_(simple)(int32_t, kill_count) \
	_(simple)(int32_t, score)

CREATE_PACKET(TS_SC_HUNTAHOLIC_UPDATE_SCORE, 4007);
#undef TS_SC_HUNTAHOLIC_UPDATE_SCORE_DEF

