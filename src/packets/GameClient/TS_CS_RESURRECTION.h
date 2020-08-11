#pragma once

#include "Packet/PacketDeclaration.h"

enum TS_RESURRECTION_TYPE : int8_t
{
	RT_UseNone = 0,
	RT_UseState = 1,
	RT_UsePotion = 2,
	RT_Compete = 3,
	RT_Deathmatch = 4
};

#define TS_CS_RESURRECTION_DEF(_) \
	_(simple)(ar_handle_t, handle, version >= EPIC_4_1) \
	_(simple)(TS_RESURRECTION_TYPE, type, version >= EPIC_6_1) \
	_(simple)(bool, use_state, version < EPIC_6_1) \
	_(simple)(bool, use_potion, version >= EPIC_4_1 && version < EPIC_6_1)

#define TS_CS_RESURRECTION_ID(X) \
	X(513, version < EPIC_9_6_3) \
	X(1513, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_RESURRECTION, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_RESURRECTION_DEF

