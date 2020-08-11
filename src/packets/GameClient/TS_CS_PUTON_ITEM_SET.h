#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_PUTON_ITEM_SET_DEF(_) \
	_(def)(array) (ar_handle_t, handle, 24) \
	  _(impl)(array)(ar_handle_t, handle, 24, version >= EPIC_4_1) \
	  _(impl)(array)(ar_handle_t, handle, 14, version < EPIC_4_1)

#define TS_CS_PUTON_ITEM_SET_ID(X) \
	X(281, version < EPIC_9_6_3) \
	X(1281, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_PUTON_ITEM_SET, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_PUTON_ITEM_SET_DEF

