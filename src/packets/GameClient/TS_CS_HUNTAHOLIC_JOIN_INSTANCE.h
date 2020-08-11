#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HUNTAHOLIC_JOIN_INSTANCE_DEF(_) \
	_(simple)(int32_t, instance_no) \
	_(string)(password, 17)

#define TS_CS_HUNTAHOLIC_JOIN_INSTANCE_ID(X) \
	X(4004, true)

CREATE_PACKET_VER_ID(TS_CS_HUNTAHOLIC_JOIN_INSTANCE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_HUNTAHOLIC_JOIN_INSTANCE_DEF

