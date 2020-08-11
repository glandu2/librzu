#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_STORAGE_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int8_t, mode) \
	_(def)(simple) (int64_t, count) \
	_(impl)(simple)(int64_t, count, version >= EPIC_4_1_1) \
	_(impl)(simple)(uint32_t, count, version < EPIC_4_1_1)

#define TS_CS_STORAGE_ID(X) \
	X(212, version < EPIC_9_6_3) \
	X(1212, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_STORAGE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_STORAGE_DEF

