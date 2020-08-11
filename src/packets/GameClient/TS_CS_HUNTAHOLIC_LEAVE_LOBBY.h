#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HUNTAHOLIC_LEAVE_LOBBY_DEF(_)

#define TS_CS_HUNTAHOLIC_LEAVE_LOBBY_ID(X) \
	X(4008, true)

CREATE_PACKET_VER_ID(TS_CS_HUNTAHOLIC_LEAVE_LOBBY, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_HUNTAHOLIC_LEAVE_LOBBY_DEF

