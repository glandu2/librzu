#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_INVENTORY.h"

#define TS_BOOTH_ITEM_INFO_DEF(_) \
	_(simple)(TS_ITEM_BASE_INFO, item) \
	_(def)(simple)(int64_t, gold) \
	_(impl)(simple)(int64_t, gold, version >= EPIC_4_1_1) \
	_(impl)(simple)(int32_t, gold, version < EPIC_4_1_1)
CREATE_STRUCT(TS_BOOTH_ITEM_INFO);
#undef TS_BOOTH_ITEM_INFO_DEF

#define TS_SC_WATCH_BOOTH_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(simple)(uint8_t, type) \
	_(count) (uint16_t, items) \
	_(dynarray)(TS_BOOTH_ITEM_INFO, items)

#define TS_SC_WATCH_BOOTH_ID(X) \
	X(703, version < EPIC_9_6_3) \
	X(1703, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_WATCH_BOOTH, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_WATCH_BOOTH_DEF

