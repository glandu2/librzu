#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SHOW_CREATE_GUILD_DEF(_)

#define TS_SC_SHOW_CREATE_GUILD_ID(X) \
	X(650, version < EPIC_9_6_3) \
	X(1650, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SHOW_CREATE_GUILD, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SHOW_CREATE_GUILD_DEF

