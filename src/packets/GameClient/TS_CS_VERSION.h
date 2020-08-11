#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_VERSION_DEF(_) \
	_(string)(szVersion, 20) \
	_(array)(uint8_t, checksum, 256, version >= EPIC_9_5_2)

#define TS_CS_VERSION_ID(X) \
	X(50, version < EPIC_7_4) \
	X(51, version >= EPIC_7_4 && version < EPIC_9_6_3) \
	X(1051, version >= EPIC_9_6_3) \

CREATE_PACKET_VER_ID(TS_CS_VERSION, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_VERSION_DEF

