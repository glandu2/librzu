#ifndef PACKETS_TS_SC_HPMP_H
#define PACKETS_TS_SC_HPMP_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_HPMP_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(simple)(int32_t, add_hp) \
	_(simple)(int32_t, hp) \
	_(simple)(int32_t, max_hp) \
	_(simple)(int32_t, add_mp) \
	_(simple)(int32_t, mp) \
	_(simple)(int32_t, max_mp) \
	_(simple)(bool, need_to_display)

CREATE_PACKET(TS_SC_HPMP, 509);

#endif // PACKETS_TS_SC_HPMP_H
