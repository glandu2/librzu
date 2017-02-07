#ifndef PACKETS_TS_SC_BONUS_EXP_JP_H
#define PACKETS_TS_SC_BONUS_EXP_JP_H

#include "Packet/PacketDeclaration.h"

#define TS_BONUS_INFO_DEF(_) \
	_(simple)(int32_t, type) \
	_(simple)(int32_t, rate) \
	_(simple)(int64_t, exp) \
	_(simple)(int32_t, jp)

CREATE_STRUCT(TS_BONUS_INFO);

#define TS_SC_BONUS_EXP_JP_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(count)(uint16_t, bonus) \
	_(dynarray)(TS_BONUS_INFO, bonus)

CREATE_PACKET(TS_SC_BONUS_EXP_JP, 1004);

#endif // PACKETS_TS_SC_BONUS_EXP_JP_H
