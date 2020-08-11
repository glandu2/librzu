#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SOULSTONE_CRAFT_DEF(_) \
	_(simple)(ar_handle_t, craft_item_handle) \
	_(array)(ar_handle_t, soulstone_handle, 4)

#define TS_CS_SOULSTONE_CRAFT_ID(X) \
	X(260, version < EPIC_9_6_3) \
	X(1260, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SOULSTONE_CRAFT, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SOULSTONE_CRAFT_DEF

