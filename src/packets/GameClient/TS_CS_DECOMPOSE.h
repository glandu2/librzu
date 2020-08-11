#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_DECOMPOSE_ITEM_INFO_DEF(_) \
	_(simple)(ar_handle_t, items_handle) \
	_(simple)(uint32_t, count, version >= EPIC_8_2, 1)
CREATE_STRUCT(TS_DECOMPOSE_ITEM_INFO);
#undef TS_DECOMPOSE_ITEM_INFO_DEF

#define TS_CS_DECOMPOSE_DEF(_) \
	_(count)(uint32_t, items) \
	_(dynarray)(TS_DECOMPOSE_ITEM_INFO, items)

// Since EPIC_8_1
#define TS_CS_DECOMPOSE_ID(X) \
	X(265, version < EPIC_9_6_3) \
	X(1265, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_DECOMPOSE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_DECOMPOSE_DEF

