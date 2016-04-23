#ifndef PACKETS_TS_SC_MOVE_H
#define PACKETS_TS_SC_MOVE_H

#include "Packet/PacketDeclaration.h"

#define MOVE_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_(float, tx) \
	simple_(float, ty)

CREATE_STRUCT(MOVE_INFO);

#define TS_SC_MOVE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_(uint32_t, start_time) \
	simple_(uint32_t, handle) \
	simple_(char, tlayer) \
	simple_(char, speed) \
	count_ (uint16_t, count, move_infos) \
	dynarray_(MOVE_INFO, move_infos)

CREATE_PACKET(TS_SC_MOVE, 8);

#endif // PACKETS_TS_SC_MOVE_H
