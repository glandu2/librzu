#ifndef PACKETS_TS_SC_REGEN_HPMP_H
#define PACKETS_TS_SC_REGEN_HPMP_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_REGEN_HPMP_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(simple)(int32_t, hp_regen) \
	_(simple)(int32_t, mp_regen) \
	_(simple)(int32_t, hp) \
	_(simple)(int32_t, mp)

CREATE_PACKET(TS_SC_REGEN_HPMP, 516);

#endif // PACKETS_TS_SC_REGEN_HPMP_H
