#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_RETURN_LOBBY_DEF(_)

#define TS_CS_REQUEST_RETURN_LOBBY_ID(X) \
	X(25, version < EPIC_9_6_3) \
	X(1025, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_REQUEST_RETURN_LOBBY, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REQUEST_RETURN_LOBBY_DEF

