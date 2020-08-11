#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HUNTAHOLIC_CREATE_INSTANCE_DEF(_) \
	_(string)(name, 31) \
	_(simple)(int8_t, max_member_count) \
	_(string)(password, 17)

#define TS_CS_HUNTAHOLIC_CREATE_INSTANCE_ID(X) \
	X(4003, true)

CREATE_PACKET_VER_ID(TS_CS_HUNTAHOLIC_CREATE_INSTANCE, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_HUNTAHOLIC_CREATE_INSTANCE_DEF

