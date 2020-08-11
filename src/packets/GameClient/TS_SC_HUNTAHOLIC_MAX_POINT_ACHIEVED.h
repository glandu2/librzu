#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HUNTAHOLIC_MAX_POINT_ACHIEVED_DEF(_)

#define TS_SC_HUNTAHOLIC_MAX_POINT_ACHIEVED_ID(X) \
	X(4010, true)

CREATE_PACKET_VER_ID(TS_SC_HUNTAHOLIC_MAX_POINT_ACHIEVED, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HUNTAHOLIC_MAX_POINT_ACHIEVED_DEF

