#ifndef PACKETS_TS_SC_EXP_UPDATE_H
#define PACKETS_TS_SC_EXP_UPDATE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_EXP_UPDATE_DEF(_) \
	_(simple) (uint32_t, handle) \
	_(simple) (uint64_t, exp) \
	_(simple)(def) (uint64_t, jp) \
	_(simple)(impl)(uint64_t, jp, version >= EPIC_7_3) \
	_(simple)(impl)(uint32_t, jp, version < EPIC_7_3)

CREATE_PACKET(TS_SC_EXP_UPDATE, 1003);

#endif // PACKETS_TS_SC_EXP_UPDATE_H
