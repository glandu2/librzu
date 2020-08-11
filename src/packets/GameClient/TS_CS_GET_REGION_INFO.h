#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_GET_REGION_INFO_DEF(_) \
	_(simple)(float, x) \
	_(simple)(float, y)

#define TS_CS_GET_REGION_INFO_ID(X) \
	X(550, version < EPIC_9_6_3) \
	X(1550, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_GET_REGION_INFO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_GET_REGION_INFO_DEF

