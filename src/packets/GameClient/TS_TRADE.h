#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_INVENTORY.h"

#define TS_TRADE_DEF(_) \
	_(simple)(ar_handle_t, target_player) \
	_(simple)(int8_t, mode) \
	_(simple)(TS_ITEM_INFO, item_info)

#define TS_TRADE_ID(X) \
	X(280, version < EPIC_9_6_3) \
	X(1280, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_TRADE, SessionType::GameClient, SessionPacketOrigin::Any);
#undef TS_TRADE_DEF

