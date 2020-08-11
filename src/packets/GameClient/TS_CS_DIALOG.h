#pragma once

#include "Packet/PacketDeclaration.h"

// Server check size - 9 == trigger_length
// No null terminator allowed
#define TS_CS_DIALOG_DEF(_) \
	_(count)(uint16_t, trigger) \
	_(dynstring)(trigger, false)

#define TS_CS_DIALOG_ID(X) \
	X(3001, true)

CREATE_PACKET_VER_ID(TS_CS_DIALOG, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_DIALOG_DEF

