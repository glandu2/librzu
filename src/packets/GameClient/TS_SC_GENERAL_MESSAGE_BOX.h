#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_GENERAL_MESSAGE_BOX_DEF(_) \
	_(count)(uint16_t, text) \
	_(dynstring)(text, false)

// Since EPIC_7_4
#define TS_SC_GENERAL_MESSAGE_BOX_ID(X) \
	X(3004, true)

CREATE_PACKET_VER_ID(TS_SC_GENERAL_MESSAGE_BOX, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_GENERAL_MESSAGE_BOX_DEF

