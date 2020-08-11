#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HUNTAHOLIC_UPDATE_SCORE_DEF(_) \
	_(simple)(int32_t, kill_count) \
	_(simple)(int32_t, score)

#define TS_SC_HUNTAHOLIC_UPDATE_SCORE_ID(X) \
	X(4007, true)

CREATE_PACKET_VER_ID(TS_SC_HUNTAHOLIC_UPDATE_SCORE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HUNTAHOLIC_UPDATE_SCORE_DEF

