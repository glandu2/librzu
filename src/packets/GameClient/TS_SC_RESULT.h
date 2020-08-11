#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_RESULT_DEF(_) \
	_(simple)(uint16_t, request_msg_id) \
	_(simple)(uint16_t, result) \
	_(simple)(int32_t, value)

#define TS_SC_RESULT_ID(X) \
	X(0, version < EPIC_9_6_3) \
	X(1000, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_RESULT, SessionType::Any, SessionPacketOrigin::Server);
#undef TS_SC_RESULT_DEF

