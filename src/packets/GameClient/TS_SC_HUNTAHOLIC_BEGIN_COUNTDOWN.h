#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HUNTAHOLIC_BEGIN_COUNTDOWN_DEF(_)

// Since EPIC_6_3
#define TS_SC_HUNTAHOLIC_BEGIN_COUNTDOWN_ID(X) \
	X(4012, true)

CREATE_PACKET_VER_ID(TS_SC_HUNTAHOLIC_BEGIN_COUNTDOWN, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HUNTAHOLIC_BEGIN_COUNTDOWN_DEF

