#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_BOOTH_OPEN_ITEM_INFO_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int32_t, cnt) \
	_(def)(simple) (int64_t, gold) \
	_(impl)(simple)(int64_t, gold, version >= EPIC_4_1_1) \
	_(impl)(simple)(int32_t, gold, version < EPIC_4_1_1)
CREATE_STRUCT(TS_BOOTH_OPEN_ITEM_INFO);
#undef TS_BOOTH_OPEN_ITEM_INFO_DEF

#define TS_CS_START_BOOTH_DEF(_) \
	_(string)(name, 49) \
	_(simple)(uint8_t, type) \
	_(count) (uint16_t, items) \
	_(dynarray)(TS_BOOTH_OPEN_ITEM_INFO, items)

#define TS_CS_START_BOOTH_ID(X) \
	X(700, version < EPIC_9_6_3) \
	X(1700, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_START_BOOTH, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_START_BOOTH_DEF

