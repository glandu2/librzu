#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_ITEM_POSITION_DEF(_) \
	_(simple)(bool, is_storage) \
	_(simple)(ar_handle_t, item_handle_1) \
	_(simple)(ar_handle_t, item_handle_2)

#define TS_CS_CHANGE_ITEM_POSITION_ID(X) \
	X(218, version < EPIC_9_6_3) \
	X(1218, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CHANGE_ITEM_POSITION, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CHANGE_ITEM_POSITION_DEF

