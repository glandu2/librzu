#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_TITLE_CONDITION_INFO_DEF(_) \
	_(simple)(int32_t, type) \
	_(simple)(int64_t, count)

CREATE_STRUCT(TS_TITLE_CONDITION_INFO);
#undef TS_TITLE_CONDITION_INFO_DEF

#define TS_SC_TITLE_CONDITION_LIST_DEF(_) \
	_(count)(uint16_t, title_condition_infos) \
	_(dynarray)(TS_TITLE_CONDITION_INFO, title_condition_infos)

// Since EPIC_8_1
#define TS_SC_TITLE_CONDITION_LIST_ID(X) \
	X(626, version < EPIC_9_6_3) \
	X(1626, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_TITLE_CONDITION_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_TITLE_CONDITION_LIST_DEF

