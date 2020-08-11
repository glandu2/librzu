#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_INVENTORY.h"

#define TS_ITEM_KEEPING_INFO_DEF(_) \
	_(simple) (int32_t, keeping_uid) \
	_(simple) (TS_ITEM_BASE_INFO, item_info) \
	_(simple) (int32_t, duration) \
	_(simple) (uint8_t, keeping_type) \
	_(simple) (int32_t, related_item_code) \
	_(simple) (int32_t, related_item_enhance) \
	_(simple) (int32_t, related_item_level)
CREATE_STRUCT(TS_ITEM_KEEPING_INFO);
#undef TS_ITEM_KEEPING_INFO_DEF

#define TS_SC_ITEM_KEEPING_LIST_DEF(_) \
	_(simple)(int32_t, page_num) \
	_(simple)(int32_t, total_page_count) \
	_(simple)(int32_t, keeping_info_count) \
	_(array) (TS_ITEM_KEEPING_INFO, keeping_info, 40)

#define TS_SC_ITEM_KEEPING_LIST_ID(X) \
	X(1351, version < EPIC_9_6_3) \
	X(2351, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ITEM_KEEPING_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ITEM_KEEPING_LIST_DEF

