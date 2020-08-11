#pragma once

#include "Packet/PacketDeclaration.h"
#include "TS_SC_CHARACTER_LIST.h"

#define TS_CS_CREATE_CHARACTER_DEF(_) \
	_(simple) (LOBBY_CHARACTER_INFO, character)

#define TS_CS_CREATE_CHARACTER_ID(X) \
	X(2002, version < EPIC_9_6_3) \
	X(2402, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_CREATE_CHARACTER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_CREATE_CHARACTER_DEF

