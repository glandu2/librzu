#ifndef PACKETS_TS_SC_REGEN_HPMP_H
#define PACKETS_TS_SC_REGEN_HPMP_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_REGEN_HPMP_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(def)(simple)(int32_t, hp_regen) \
	_(impl)(simple)(int32_t, hp_regen, version >= EPIC_7_1) \
	_(impl)(simple)(int16_t, hp_regen, version < EPIC_7_1) \
	_(def)(simple)(int32_t, mp_regen) \
	_(impl)(simple)(int32_t, mp_regen, version >= EPIC_7_1) \
	_(impl)(simple)(int16_t, mp_regen, version < EPIC_7_1) \
	_(simple)(int32_t, hp) \
	_(def)(simple)(int32_t, mp) \
	_(impl)(simple)(int32_t, mp, version >= EPIC_7_1) \
	_(impl)(simple)(int16_t, mp, version < EPIC_7_1) \

CREATE_PACKET(TS_SC_REGEN_HPMP, 516);

#endif // PACKETS_TS_SC_REGEN_HPMP_H
