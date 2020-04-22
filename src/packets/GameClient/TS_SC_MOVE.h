#pragma once

#include "Packet/PacketDeclaration.h"

#define MOVE_INFO_DEF(_) \
	_(simple)(float, tx) \
	_(simple)(float, ty)

CREATE_STRUCT(MOVE_INFO);
#undef MOVE_INFO_DEF

#define TS_SC_MOVE_DEF(_) \
	_(simple)(ar_time_t, start_time) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int8_t, tlayer) \
	_(simple)(uint8_t, speed) \
	_(count) (uint16_t, move_infos) \
	_(dynarray)(MOVE_INFO, move_infos)

CREATE_PACKET(TS_SC_MOVE, 8);
#undef TS_SC_MOVE_DEF

