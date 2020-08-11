#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_HUNTAHOLIC_INSTANCE_LIST_DEF(_) \
	_(simple)(int32_t, page)

#define TS_CS_HUNTAHOLIC_INSTANCE_LIST_ID(X) \
	X(4000, true)

CREATE_PACKET_VER_ID(TS_CS_HUNTAHOLIC_INSTANCE_LIST, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_HUNTAHOLIC_INSTANCE_LIST_DEF

