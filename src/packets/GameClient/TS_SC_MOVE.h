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

#define TS_SC_MOVE_ID(X) \
	X(8, version < EPIC_9_6_3) \
	X(1008, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_MOVE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_MOVE_DEF

