#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_PUTOFF_ITEM_DEF(_) \
	_(simple) (int8_t, position) \
	_(simple) (ar_handle_t, target_handle)

#define TS_CS_PUTOFF_ITEM_ID(X) \
	X(201, version < EPIC_9_6_3) \
	X(1201, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_PUTOFF_ITEM, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_PUTOFF_ITEM_DEF

