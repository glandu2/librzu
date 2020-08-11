#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HUNTAHOLIC_BEGIN_HUNTING_DEF(_) \
	_(simple)(ar_time_t, begin_time)

#define TS_SC_HUNTAHOLIC_BEGIN_HUNTING_ID(X) \
	X(4009, true)

CREATE_PACKET_VER_ID(TS_SC_HUNTAHOLIC_BEGIN_HUNTING, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HUNTAHOLIC_BEGIN_HUNTING_DEF

