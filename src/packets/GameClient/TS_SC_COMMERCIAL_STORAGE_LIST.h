#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_COMMERCIAL_ITEM_INFO_DEF(_) \
	_(simple)(uint32_t, commercial_item_uid) \
	_(simple)(int32_t, code) \
	_(simple)(uint16_t, count)

CREATE_STRUCT(TS_COMMERCIAL_ITEM_INFO);
#undef TS_COMMERCIAL_ITEM_INFO_DEF

#define TS_SC_COMMERCIAL_STORAGE_LIST_DEF(_) \
	_(count)(uint16_t, items) \
	_(dynarray)(TS_COMMERCIAL_ITEM_INFO, items)

#define TS_SC_COMMERCIAL_STORAGE_LIST_ID(X) \
	X(10004, version < EPIC_9_6_3) \
	X(9004, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_COMMERCIAL_STORAGE_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_COMMERCIAL_STORAGE_LIST_DEF

