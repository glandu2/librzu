#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_DEATHMATCH_RANKING.h"

#define TS_SC_DEATHMATCH_RANKING_OPEN_DEF(_) \
	_(array)(TS_DEATHMATCH_RANKING_INFO, players, 5)

#define TS_SC_DEATHMATCH_RANKING_OPEN_ID(X) \
	X(4300, true)

CREATE_PACKET_VER_ID(TS_SC_DEATHMATCH_RANKING_OPEN, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DEATHMATCH_RANKING_OPEN_DEF

