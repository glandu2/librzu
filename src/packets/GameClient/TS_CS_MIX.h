#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_MIX_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(uint16_t, count)

CREATE_STRUCT(TS_MIX_INFO);
#undef TS_MIX_INFO_DEF

#define TS_CS_MIX_DEF(_) \
	_(simple)(TS_MIX_INFO, main_item) \
	_(count)(uint16_t, sub_items) \
	_(dynarray)(TS_MIX_INFO, sub_items)

#define TS_CS_MIX_ID(X) \
	X(256, version < EPIC_9_6_3) \
	X(1256, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_MIX, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_MIX_DEF

