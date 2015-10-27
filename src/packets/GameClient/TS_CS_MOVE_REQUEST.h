#ifndef PACKETS_TS_CS_MOVE_REQUEST_H
#define PACKETS_TS_CS_MOVE_REQUEST_H

#include "PacketDeclaration.h"

#define MOVE_INFO_DEF(simple_, array_, dynarray_, count_) \
	simple_(float, tx) \
	simple_(float, ty)

CREATE_STRUCT(MOVE_INFO);

#define TS_CS_MOVE_REQUEST_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint32_t, handle) \
	simple_(float, x) \
	simple_(float, y) \
	simple_(float, y) \
	simple_(uint32_t, cur_time) \
	simple_(bool, speed_sync) \
	simple_(uint16_t, count) \
	dynarray_(MOVE_INFO, move_infos)

CREATE_PACKET(TS_CS_MOVE_REQUEST, 5);

#endif // PACKETS_TS_CS_MOVE_REQUEST_H