#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_GAME_GUARD_AUTH_QUERY.h"

#define TS_CS_GAME_GUARD_AUTH_ANSWER_DEF(_) \
	_(simple)(TS_GAME_GUARD_AUTH, auth)

#define TS_CS_GAME_GUARD_AUTH_ANSWER_ID(X) \
	X(56, version < EPIC_9_6_3) \
	X(1056, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_GAME_GUARD_AUTH_ANSWER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_GAME_GUARD_AUTH_ANSWER_DEF

