#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_OPEN_URL_DEF(_) \
	_(simple)(bool, wait_for_event_scene) \
	_(simple)(int32_t, width) \
	_(simple)(int32_t, height) \
	_(count)(uint16_t, url) \
	_(dynstring)(url, false)

#define TS_SC_OPEN_URL_ID(X) \
	X(9000, version < EPIC_9_6_3) \
	X(8100, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_OPEN_URL, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_OPEN_URL_DEF

