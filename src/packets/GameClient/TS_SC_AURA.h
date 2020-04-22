#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_AURA_DEF(_) \
	_(simple)(ar_handle_t, caster) \
	_(simple)(uint16_t, skill_id) \
	_(simple)(bool, status)

CREATE_PACKET(TS_SC_AURA, 407);
#undef TS_SC_AURA_DEF

