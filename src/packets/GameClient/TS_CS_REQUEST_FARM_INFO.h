#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_REQUEST_FARM_INFO_DEF(_)

// Since EPIC_7_3
#define TS_CS_REQUEST_FARM_INFO_ID(X) \
	X(6000, true)

CREATE_PACKET_VER_ID(TS_CS_REQUEST_FARM_INFO, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_REQUEST_FARM_INFO_DEF

