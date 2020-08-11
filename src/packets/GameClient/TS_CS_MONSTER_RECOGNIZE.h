#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_MONSTER_RECOGNIZE_DEF(_) \
	_(simple)(ar_handle_t, recognizer_handle) \
	_(simple)(ar_handle_t, monster_handle)

// Since EPIC_6_3
#define TS_CS_MONSTER_RECOGNIZE_ID(X) \
	X(517, version < EPIC_9_6_3) \
	X(1517, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_MONSTER_RECOGNIZE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_MONSTER_RECOGNIZE_DEF

