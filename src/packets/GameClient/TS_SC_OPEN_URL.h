#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_OPEN_URL_DEF(_) \
	_(simple)(bool, wait_for_event_scene) \
	_(simple)(int32_t, width) \
	_(simple)(int32_t, height) \
	_(count)(uint16_t, url) \
	_(dynstring)(url, false)

CREATE_PACKET(TS_SC_OPEN_URL, 9000);
#undef TS_SC_OPEN_URL_DEF

