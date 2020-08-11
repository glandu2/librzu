#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_BATTLE_ARENA_ABSENCE_CHECK_ANSWER_DEF(_) \
	_(simple)(bool, bSuccess)

// Since EPIC_8_1
#define TS_CS_BATTLE_ARENA_ABSENCE_CHECK_ANSWER_ID(X) \
	X(4719, true)

CREATE_PACKET_VER_ID(TS_CS_BATTLE_ARENA_ABSENCE_CHECK_ANSWER, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_BATTLE_ARENA_ABSENCE_CHECK_ANSWER_DEF

