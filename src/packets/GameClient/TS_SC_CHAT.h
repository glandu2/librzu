#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_CS_CHAT_REQUEST.h"

// Message len include null terminator
// Message must have null terminator
#define TS_SC_CHAT_DEF(_) \
	_(string)(szSender, 21) \
	_(count)(uint16_t, message) \
	_(simple)(TS_CHAT_TYPE, type) \
	_(dynstring)(message, true)

#define TS_SC_CHAT_ID(X) \
	X(22, version < EPIC_9_6_3) \
	X(1022, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CHAT, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CHAT_DEF

