#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_GAME_GUARD_AUTH_QUERY.h"

#define TS_CS_GAME_GUARD_AUTH_ANSWER_DEF(_) \
	_(simple)(TS_GAME_GUARD_AUTH, auth)

CREATE_PACKET(TS_CS_GAME_GUARD_AUTH_ANSWER, 56);
#undef TS_CS_GAME_GUARD_AUTH_ANSWER_DEF

