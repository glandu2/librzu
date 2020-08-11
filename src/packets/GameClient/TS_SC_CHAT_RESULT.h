#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHAT_RESULT_DEF(_) \
	_(simple)(int8_t, type) \
	_(simple)(int8_t, percentage) \
	_(simple)(int32_t, result) \
	_(simple)(int32_t, reserved)

#define TS_SC_CHAT_RESULT_ID(X) \
	X(24, version < EPIC_9_6_3) \
	X(1024, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CHAT_RESULT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CHAT_RESULT_DEF

