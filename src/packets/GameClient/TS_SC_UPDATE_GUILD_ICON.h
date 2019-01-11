#ifndef PACKETS_TS_SC_UPDATE_GUILD_ICON_H
#define PACKETS_TS_SC_UPDATE_GUILD_ICON_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_UPDATE_GUILD_ICON_DEF(_) \
	_(simple)(uint32_t, client_id) \
	_(simple)(uint32_t, account_id) \
	_(simple)(uint32_t, one_time_password) \
	_(string)(raw_server_name, 32)

CREATE_PACKET(TS_SC_UPDATE_GUILD_ICON, 652);

#endif // PACKETS_TS_SC_UPDATE_GUILD_ICON_H
