#ifndef PACKETS_TS_CS_ATTACK_REQUEST_H
#define PACKETS_TS_CS_ATTACK_REQUEST_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_ATTACK_REQUEST_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(ar_handle_t, target_handle)

CREATE_PACKET(TS_CS_ATTACK_REQUEST, 100);

#endif // PACKETS_TS_CS_ATTACK_REQUEST_H
