#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CANT_ATTACK_DEF(_) \
	_(simple)(ar_handle_t, attacker_handle) \
	_(simple)(ar_handle_t, target_handle) \
	_(simple)(int32_t, reason)

CREATE_PACKET(TS_SC_CANT_ATTACK, 102);
#undef TS_SC_CANT_ATTACK_DEF

