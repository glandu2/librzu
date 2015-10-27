#ifndef PACKETS_TS_SC_GOLD_UPDATE_H
#define PACKETS_TS_SC_GOLD_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_GOLD_UPDATE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint64_t, gold) \
	simple_ (uint32_t, chaos)

CREATE_PACKET(TS_SC_GOLD_UPDATE, 1001);

#endif // PACKETS_TS_SC_GOLD_UPDATE_H
