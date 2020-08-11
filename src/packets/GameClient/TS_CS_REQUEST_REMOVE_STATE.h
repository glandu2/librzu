#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_REMOVE_STATE_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(simple)(int32_t, state_code)

#define TS_CS_REQUEST_REMOVE_STATE_ID(X) \
	X(408, version < EPIC_9_6_3) \
	X(1408, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_REQUEST_REMOVE_STATE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REQUEST_REMOVE_STATE_DEF

