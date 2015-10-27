#ifndef PACKETS_TS_SC_EXP_UPDATE_H
#define PACKETS_TS_SC_EXP_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_EXP_UPDATE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	simple_ (uint64_t, exp) \
	simple_ (uint64_t, jp)

CREATE_PACKET(TS_SC_EXP_UPDATE, 1003);

#endif // PACKETS_TS_SC_EXP_UPDATE_H
