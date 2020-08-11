#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HUNTAHOLIC_BEGIN_HUNTING_DEF(_)

// Since EPIC_6_3
#define TS_CS_HUNTAHOLIC_BEGIN_HUNTING_ID(X) \
	X(4011, true)

CREATE_PACKET_VER_ID(TS_CS_HUNTAHOLIC_BEGIN_HUNTING, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_HUNTAHOLIC_BEGIN_HUNTING_DEF

