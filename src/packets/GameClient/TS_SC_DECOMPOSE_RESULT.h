#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_DECOMPOSE_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(uint32_t, count)

CREATE_STRUCT(TS_DECOMPOSE_INFO);
#undef TS_DECOMPOSE_INFO_DEF

#define TS_SC_DECOMPOSE_RESULT_DEF(_) \
	_(count)(uint32_t, decompositions) \
	_(dynarray)(TS_DECOMPOSE_INFO, decompositions)

// Since EPIC_8_1
#define TS_SC_DECOMPOSE_RESULT_ID(X) \
	X(266, version < EPIC_9_6_3) \
	X(1266, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_DECOMPOSE_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DECOMPOSE_RESULT_DEF

