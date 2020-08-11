#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_INVENTORY.h"

#define TS_FARM_SUMMON_INFO_DEF(_) \
	_(simple)(int32_t, index) \
	_(simple)(int64_t, exp) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(int32_t, duration) \
	_(simple)(int32_t, elasped_time) \
	_(simple)(int32_t, refresh_time) \
	_(simple)(int8_t, using_cash) \
	_(simple)(int8_t, using_cracker) \
	_(simple)(TS_ITEM_BASE_INFO, card_info)

CREATE_STRUCT(TS_FARM_SUMMON_INFO);
#undef TS_FARM_SUMMON_INFO_DEF

#define TS_SC_FARM_INFO_DEF(_) \
	_(count)(int8_t, summons) \
	_(dynarray)(TS_FARM_SUMMON_INFO, summons)

// Since EPIC_7_3
#define TS_SC_FARM_INFO_ID(X) \
	X(6001, true)

CREATE_PACKET_VER_ID(TS_SC_FARM_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_FARM_INFO_DEF

