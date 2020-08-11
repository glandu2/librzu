#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_MIX_TYPE : int8_t
{
	MIX_TYPE_NONE = 0,
	MIX_TYPE_AWAKEN = 1
};

#define TS_SC_MIX_RESULT_DEF(_) \
	_(count)(uint32_t, handles) \
	_(simple)(TS_MIX_TYPE, type, version >= EPIC_8_1, MIX_TYPE_NONE) \
	_(dynarray)(ar_handle_t, handles)

#define TS_SC_MIX_RESULT_ID(X) \
	X(257, version < EPIC_9_6_3) \
	X(1257, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_MIX_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_MIX_RESULT_DEF

