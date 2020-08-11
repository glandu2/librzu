#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HUNTAHOLIC_LEAVE_INSTANCE_DEF(_)

#define TS_CS_HUNTAHOLIC_LEAVE_INSTANCE_ID(X) \
	X(4005, true)

CREATE_PACKET_VER_ID(TS_CS_HUNTAHOLIC_LEAVE_INSTANCE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_HUNTAHOLIC_LEAVE_INSTANCE_DEF

