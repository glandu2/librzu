#ifndef PACKETS_TS_SC_CANT_ATTACK_H
#define PACKETS_TS_SC_CANT_ATTACK_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_CANT_ATTACK_DEF(_) \
	_(simple)(ar_handle_t, attacker_handle) \
	_(simple)(ar_handle_t, target_handle) \
	_(simple)(int32_t, reason)

CREATE_PACKET(TS_SC_CANT_ATTACK, 102);

#endif // PACKETS_TS_SC_CANT_ATTACK_H
