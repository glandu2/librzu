#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SHOW_WINDOW_DEF(_) \
	_(count)(uint16_t, window) \
	_(count)(uint16_t, argument, version >= EPIC_5_1) \
	_(count)(uint16_t, trigger) \
	_(dynstring)(window, false) \
	_(dynstring)(argument, false, version >= EPIC_5_1) \
	_(dynstring)(trigger, false)

#define TS_SC_SHOW_WINDOW_ID(X) \
	X(3003, true)

CREATE_PACKET_VER_ID(TS_SC_SHOW_WINDOW, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SHOW_WINDOW_DEF

