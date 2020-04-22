#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_ERASE_ITEM_INFO_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int64_t, count, version >= EPIC_7_2) \
	_(simple)(bool, is_in_storage, version >= EPIC_7_4)

CREATE_STRUCT(TS_ERASE_ITEM_INFO);
#undef TS_ERASE_ITEM_INFO_DEF

#define TS_CS_ERASE_ITEM_DEF(_) \
	_(count)(int8_t, items, version >= EPIC_7_2, 1) \
	_(dynarray)(TS_ERASE_ITEM_INFO, items)

CREATE_PACKET(TS_CS_ERASE_ITEM, 208);
#undef TS_CS_ERASE_ITEM_DEF

