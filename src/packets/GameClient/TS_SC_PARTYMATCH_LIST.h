#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_GROUP_FINDER_GROUP_DEF(_) \
	_(simple)(int32_t, index) \
	_(simple)(ar_handle_t, group_handle) \
	_(simple)(int16_t, zone) \
	_(simple)(int16_t, min_level) \
	_(simple)(int16_t, player_num) \
	_(simple)(int16_t, max_player_num) \
	_(string)(description, 32)
CREATE_STRUCT(TS_GROUP_FINDER_GROUP);
#undef TS_GROUP_FINDER_GROUP_DEF

#define TS_SC_PARTYMATCH_LIST_DEF(_) \
	_(simple)(int32_t, page_num) \
	_(simple)(int32_t, total_group_num) \
	_(array)(TS_GROUP_FINDER_GROUP, groups, 10)

#define TS_SC_PARTYMATCH_LIST_ID(X) \
	X(7001, true)

CREATE_PACKET_VER_ID(TS_SC_PARTYMATCH_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_PARTYMATCH_LIST_DEF

