#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_CS_CHAT_REQUEST.h"

// Message len include null terminator
// Message must have null terminator
#define TS_SC_CHAT_LOCAL_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(count)(uint8_t, message) \
	_(simple)(TS_CHAT_TYPE, type) \
	_(dynstring)(message, true)

#define TS_SC_CHAT_LOCAL_ID(X) \
	X(21, version < EPIC_9_6_3) \
	X(1021, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CHAT_LOCAL, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CHAT_LOCAL_DEF

