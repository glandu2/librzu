#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_UPDATE_GUILD_ICON_DEF(_) \
	_(simple)(uint32_t, client_id) \
	_(simple)(uint32_t, account_id) \
	_(simple)(uint32_t, one_time_password) \
	_(string)(raw_server_name, 32)

#define TS_SC_UPDATE_GUILD_ICON_ID(X) \
	X(652, version < EPIC_9_6_3) \
	X(1652, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_UPDATE_GUILD_ICON, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_UPDATE_GUILD_ICON_DEF

