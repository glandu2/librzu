#pragma once

#include "Packet/PacketDeclaration.h"

// maybe one handle = master of group
#define TS_GROUP_FINDER_DETAIL_DEF(_) \
	_(simple)(uint16_t, job_id) \
	_(simple)(uint8_t, level) \
	_(simple)(uint8_t, flag) \
	_(simple)(ar_handle_t, handle) \
	_(string)(name, 20)
CREATE_STRUCT(TS_GROUP_FINDER_DETAIL);
#undef TS_GROUP_FINDER_DETAIL_DEF

#define TS_SC_PARTYMATCH_MEMBER_DEF(_) \
	_(simple)(int32_t, index) \
	_(simple)(ar_handle_t, master_handle) \
	_(array)(TS_GROUP_FINDER_DETAIL, members, 10)

#define TS_SC_PARTYMATCH_MEMBER_ID(X) \
	X(7002, true)

CREATE_PACKET_VER_ID(TS_SC_PARTYMATCH_MEMBER, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_PARTYMATCH_MEMBER_DEF

