#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_NPC_TRADE_INFO_DEF(_) \
	_(simple)(bool, is_sell) \
	_(simple)(int32_t, code) \
	_(simple)(int64_t, count) \
	_(simple)(int64_t, price) \
	_(simple)(int32_t, huntaholic_point, version >= EPIC_5_2) \
	_(simple)(int32_t, arena_point, version >= EPIC_8_1) \
	_(simple)(ar_handle_t, target)

#define TS_SC_NPC_TRADE_INFO_ID(X) \
	X(240, version < EPIC_9_6_3) \
	X(1240, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_NPC_TRADE_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_NPC_TRADE_INFO_DEF

