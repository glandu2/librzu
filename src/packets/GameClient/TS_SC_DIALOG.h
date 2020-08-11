#pragma once

#include "Packet/PacketDeclaration.h"

// strings len does not include null terminator
// Server does not include null terminator in strings
#define TS_SC_DIALOG_DEF(_) \
	_(simple)(int32_t, type) \
	_(simple)(ar_handle_t, npc_handle) \
	_(count)(uint16_t, title) \
	_(count)(uint16_t, text) \
	_(count)(uint16_t, menu) \
	_(count)(uint16_t, script, version >= EPIC_9_6_2) \
	_(dynstring)(title, false) \
	_(dynstring)(text, false) \
	_(dynstring)(menu, false) \
	_(dynstring)(script, false, version >= EPIC_9_6_2)

#define TS_SC_DIALOG_ID(X) \
	X(3000, true)

CREATE_PACKET_VER_ID(TS_SC_DIALOG, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_DIALOG_DEF

