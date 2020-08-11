#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_AURA_DEF(_) \
	_(simple)(ar_handle_t, caster) \
	_(simple)(uint16_t, skill_id) \
	_(simple)(bool, status)

#define TS_SC_AURA_ID(X) \
	X(407, version < EPIC_9_6_3) \
	X(1407, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_AURA, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_AURA_DEF

