#ifndef PACKETS_TS_CS_MOVE_REQUEST_H
#define PACKETS_TS_CS_MOVE_REQUEST_H

#include "Packet/PacketDeclaration.h"

#define MOVE_REQUEST_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_(float, tx) \
	simple_(float, ty)

CREATE_STRUCT(MOVE_REQUEST_INFO);

#define TS_CS_MOVE_REQUEST_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_(uint32_t, handle) \
	simple_(float, x) \
	simple_(float, y) \
	simple_(uint32_t, cur_time) \
	simple_(bool, speed_sync) \
	count_ (uint16_t, count, move_infos) \
	dynarray_(MOVE_REQUEST_INFO, move_infos)

#define TS_CS_MOVE_REQUEST_ID(X) \
	X(5, version < EPIC_9_2) \
	X(65, version >= EPIC_9_2)

CREATE_PACKET_VER_ID(TS_CS_MOVE_REQUEST);

#endif // PACKETS_TS_CS_MOVE_REQUEST_H
