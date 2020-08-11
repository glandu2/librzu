#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SELL_TO_BOOTH_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int32_t, cnt)

#define TS_CS_SELL_TO_BOOTH_ID(X) \
	X(706, version < EPIC_9_6_3) \
	X(1706, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SELL_TO_BOOTH, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SELL_TO_BOOTH_DEF

