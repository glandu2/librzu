#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_ERASE_ITEM_INFO_RESULT_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int64_t, count)

CREATE_STRUCT(TS_ERASE_ITEM_INFO_RESULT);
#undef TS_ERASE_ITEM_INFO_RESULT_DEF

#define TS_SC_ERASE_ITEM_DEF(_) \
	_(count)(int8_t, erased_items) \
	_(dynarray)(TS_ERASE_ITEM_INFO_RESULT, erased_items)

// Since EPIC_7_2, was TS_SC_RESULT before
#define TS_SC_ERASE_ITEM_ID(X) \
	X(209, version < EPIC_9_6_3) \
	X(1209, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ERASE_ITEM, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ERASE_ITEM_DEF

