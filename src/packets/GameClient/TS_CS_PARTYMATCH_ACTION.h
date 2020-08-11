#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_GROUP_FINDER_REQUEST_TYPE : int32_t
{
	GFRT_List = 0,
	GFRT_Detail = 1,
	GFRT_Add = 2,
	GFRT_Cancel = 3,
	GFRT_Apply = 4
};

#define TS_CS_PARTYMATCH_ACTION_DEF(_) \
	_(simple)(int32_t, flag) \
	_(simple)(int32_t, page_num) /* only with GFRT_List */ \
	_(simple)(int32_t, index) \
	_(simple)(uint32_t, requested_group_id) /* only with GFRT_Detail and GFRT_Apply */ \
	_(simple)(ar_handle_t, group_handle) /* only with GFRT_Add */ \
	_(simple)(int16_t, zone) /* only with GFRT_List and GFRT_Add */ \
	_(simple)(int16_t, min_level) /* only with GFRT_Add */ \
	_(simple)(int16_t, player_number) \
	_(simple)(uint16_t, max_player_number) /* only with GFRT_Add */ \
	_(string)(description, 32) /* only with GFRT_Add */

#define TS_CS_PARTYMATCH_ACTION_ID(X) \
	X(7000, true)

CREATE_PACKET_VER_ID(TS_CS_PARTYMATCH_ACTION, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_PARTYMATCH_ACTION_DEF

