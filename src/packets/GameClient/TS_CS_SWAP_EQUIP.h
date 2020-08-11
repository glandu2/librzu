#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_SWAP_EQUIP_DEF(_)

// Since EPIC_7_2
#define TS_CS_SWAP_EQUIP_ID(X) \
	X(223, version < EPIC_9_6_3) \
	X(1223, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_SWAP_EQUIP, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_SWAP_EQUIP_DEF

