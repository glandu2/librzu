#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_OPEN_GUILD_WINDOW_DEF(_) \
	_(simple)(int32_t, client_id) \
	_(simple)(int32_t, account_id) \
	_(simple)(int32_t, one_time_password) \
	_(string)(raw_server_name, 32)

#define TS_SC_OPEN_GUILD_WINDOW_ID(X) \
	X(651, version < EPIC_9_6_3) \
	X(1651, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_OPEN_GUILD_WINDOW, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_OPEN_GUILD_WINDOW_DEF

