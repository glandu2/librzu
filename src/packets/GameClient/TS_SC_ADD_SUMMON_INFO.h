#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ADD_SUMMON_INFO_DEF(_) \
	_(simple)(ar_handle_t, card_handle) \
	_(simple)(ar_handle_t, summon_handle) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(int32_t, code) \
	_(simple)(int32_t, level) \
	_(simple)(int32_t, sp)

#define TS_SC_ADD_SUMMON_INFO_ID(X) \
	X(301, version < EPIC_9_6_3) \
	X(1301, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ADD_SUMMON_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ADD_SUMMON_INFO_DEF

