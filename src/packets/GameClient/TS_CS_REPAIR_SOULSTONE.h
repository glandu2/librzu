#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REPAIR_SOULSTONE_DEF(_) \
	_(array)(ar_handle_t, item_handle, 6)

#define TS_CS_REPAIR_SOULSTONE_ID(X) \
	X(262, version < EPIC_9_6_3) \
	X(1262, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_REPAIR_SOULSTONE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REPAIR_SOULSTONE_DEF

