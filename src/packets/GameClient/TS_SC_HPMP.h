#ifndef PACKETS_TS_SC_HPMP_H
#define PACKETS_TS_SC_HPMP_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_HPMP_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_(uint32_t, handle) \
	simple_(int32_t, add_hp) \
	simple_(int32_t, hp) \
	simple_(int32_t, max_hp) \
	simple_(int32_t, add_mp) \
	simple_(int32_t, mp) \
	simple_(int32_t, max_mp) \
	simple_(bool, need_to_display)

CREATE_PACKET(TS_SC_HPMP, 509);

#endif // PACKETS_TS_SC_HPMP_H
