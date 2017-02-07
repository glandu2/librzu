#ifndef PACKETS_TS_SC_REGEN_INFO_H
#define PACKETS_TS_SC_REGEN_INFO_H

#include "Packet/PacketDeclaration.h"

// Seems unused
#define TS_SC_REGEN_INFO_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(simple)(int16_t, hp_regen_percentage) \
	_(simple)(int32_t, hp_regen_point) \
	_(simple)(int16_t, mp_regen_percentage) \
	_(simple)(int32_t, mp_regen_point)

CREATE_PACKET(TS_SC_REGEN_INFO, 510);

#endif // PACKETS_TS_SC_REGEN_INFO_H
