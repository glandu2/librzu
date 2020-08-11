#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_INSTANCE_GAME_ENTER_DEF(_) \
	_(simple)(int32_t, instance_game_type)

// Since EPIC_6_3
#define TS_CS_INSTANCE_GAME_ENTER_ID(X) \
	X(4250, true)

CREATE_PACKET_VER_ID(TS_CS_INSTANCE_GAME_ENTER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_INSTANCE_GAME_ENTER_DEF

