#ifndef PACKETS_TS_SC_DISCONNECT_DESC_H
#define PACKETS_TS_SC_DISCONNECT_DESC_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_DISCONNECT_DESC_DEF(_) \
	_(simple)(uint8_t, desc_id)

CREATE_PACKET(TS_SC_DISCONNECT_DESC, 28);

#endif // PACKETS_TS_SC_DISCONNECT_DESC_H
