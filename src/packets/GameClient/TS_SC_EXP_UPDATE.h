#ifndef PACKETS_TS_SC_EXP_UPDATE_H
#define PACKETS_TS_SC_EXP_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_EXP_UPDATE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, handle) \
	simple_ (uint64_t, exp) \
	simple_(def) (uint64_t, jp) \
	simple_(impl)(uint64_t, jp, version >= EPIC_7_3) \
	simple_(impl)(uint32_t, jp, version < EPIC_7_3)

CREATE_PACKET(TS_SC_EXP_UPDATE, 1003);

#endif // PACKETS_TS_SC_EXP_UPDATE_H
