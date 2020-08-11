#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SELL_ITEM_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(def)(simple) (int64_t, sell_count) \
	_(impl)(simple)(int64_t, sell_count, version >= EPIC_8_2) \
	_(impl)(simple)(uint16_t, sell_count, version >= EPIC_4_1 && version < EPIC_8_2) \
	_(impl)(simple)(uint8_t, sell_count, version < EPIC_4_1)

#define TS_CS_SELL_ITEM_ID(X) \
	X(252, version < EPIC_9_6_3) \
	X(1252, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SELL_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SELL_ITEM_DEF

