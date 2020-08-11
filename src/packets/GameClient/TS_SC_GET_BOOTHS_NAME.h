#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_BOOTH_NAME_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(string)(name, 49)
CREATE_STRUCT(TS_BOOTH_NAME);
#undef TS_BOOTH_NAME_DEF

#define TS_SC_GET_BOOTHS_NAME_DEF(_) \
	_(count)(int32_t, booths) \
	_(dynarray)(TS_BOOTH_NAME, booths)

#define TS_SC_GET_BOOTHS_NAME_ID(X) \
	X(708, version < EPIC_9_6_3) \
	X(1708, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_GET_BOOTHS_NAME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_GET_BOOTHS_NAME_DEF

