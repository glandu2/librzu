#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_ATTACK_REQUEST_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(ar_handle_t, target_handle)

#define TS_CS_ATTACK_REQUEST_ID(X) \
	X(100, version < EPIC_9_6_3) \
	X(1100, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_ATTACK_REQUEST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_ATTACK_REQUEST_DEF

