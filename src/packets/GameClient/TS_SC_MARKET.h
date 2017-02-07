#ifndef PACKETS_TS_SC_MARKET_H
#define PACKETS_TS_SC_MARKET_H

#include "Packet/PacketDeclaration.h"

#define TS_MARKET_ITEM_INFO_DEF(_) \
	_(simple)(int32_t, code) \
	_(simple)(int64_t, price) \
	_(simple)(int32_t, huntaholic_point) \
	_(simple)(int32_t, arena_point)

CREATE_STRUCT(TS_MARKET_ITEM_INFO);

#define TS_SC_MARKET_DEF(_) \
	_(simple)(uint32_t, npc_handle) \
	_(count)(uint16_t, items) \
	_(padmarker)(item_list_marker) \
	_(dynarray)(TS_MARKET_ITEM_INFO, items) \
	_(pad)(24 * (uint32_t)items.size(), item_list_marker)

CREATE_PACKET(TS_SC_MARKET, 250);

#endif // PACKETS_TS_SC_MARKET_H
