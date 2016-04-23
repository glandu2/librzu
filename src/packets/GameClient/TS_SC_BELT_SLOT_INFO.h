#ifndef PACKETS_TS_SC_BELT_SLOT_INFO_H
#define PACKETS_TS_SC_BELT_SLOT_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_BELT_SLOT_INFO_DEF(_) \
	_(array) (uint32_t, handle, 6) \

CREATE_PACKET(TS_SC_BELT_SLOT_INFO, 216);

#endif // PACKETS_TS_SC_BELT_SLOT_INFO_H
