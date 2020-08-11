#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CANT_ATTACK_DEF(_) \
	_(simple)(ar_handle_t, attacker_handle) \
	_(simple)(ar_handle_t, target_handle) \
	_(simple)(int32_t, reason)

#define TS_SC_CANT_ATTACK_ID(X) \
	X(102, version < EPIC_9_6_3) \
	X(1102, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CANT_ATTACK, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CANT_ATTACK_DEF

