#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_MARKET_ITEM_INFO_DEF(_) \
	_(simple)(int32_t, code) \
	_(def)(simple) (int64_t, price) \
	_(impl)(simple)(int64_t, price, version >= EPIC_4_1_1) \
	_(impl)(simple)(int32_t, price, version < EPIC_4_1_1) \
	_(simple)(int32_t, huntaholic_point, version >= EPIC_5_2) \
	_(simple)(int32_t, arena_point, version >= EPIC_8_1)

CREATE_STRUCT(TS_MARKET_ITEM_INFO);
#undef TS_MARKET_ITEM_INFO_DEF

#define TS_SC_MARKET_DEF(_) \
	_(simple)(ar_handle_t, npc_handle) \
	_(count)(uint16_t, items) \
	_(dynarray)(TS_MARKET_ITEM_INFO, items) \
	_(padmarker)(item_list_marker) \
	_(pad)(4 * (uint32_t)items.size(), item_list_marker)

#define TS_SC_MARKET_ID(X) \
	X(250, version < EPIC_9_6_3) \
	X(1250, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_MARKET, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_MARKET_DEF

