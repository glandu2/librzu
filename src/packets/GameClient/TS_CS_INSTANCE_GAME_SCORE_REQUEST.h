#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_INSTANCE_GAME_SCORE_REQUEST_DEF(_)

// Since EPIC_6_3
#define TS_CS_INSTANCE_GAME_SCORE_REQUEST_ID(X) \
	X(4252, true)

CREATE_PACKET_VER_ID(TS_CS_INSTANCE_GAME_SCORE_REQUEST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_INSTANCE_GAME_SCORE_REQUEST_DEF

