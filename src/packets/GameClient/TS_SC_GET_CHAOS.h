#ifndef PACKETS_TS_SC_GET_CHAOS_H
#define PACKETS_TS_SC_GET_CHAOS_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_GET_CHAOS_DEF(_) \
	_(simple)(uint32_t, hPlayer) \
	_(simple)(uint32_t, hCorpse) \
	_(simple)(int32_t, nChaos) \
	_(def)(simple)(int32_t, nBonusType) \
	  _(impl)(simple)(int8_t, nBonusType, version >= EPIC_4_1 && version < EPIC_9_6_2) \
	  _(impl)(simple)(int32_t, nBonusType, version >= EPIC_9_6_2) \
	_(def)(simple)(int32_t, nBonusPercent) \
	  _(impl)(simple)(int8_t, nBonusPercent, version >= EPIC_4_1 && version < EPIC_9_6_2) \
	  _(impl)(simple)(int32_t, nBonusPercent, version >= EPIC_9_6_2) \
	_(simple)(int32_t, nBonus, version >= EPIC_4_1)

CREATE_PACKET(TS_SC_GET_CHAOS, 213);

#endif // PACKETS_TS_SC_GET_CHAOS_H
