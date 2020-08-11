#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_EMOTION_DEF(_) \
	_(simple)(int32_t, emotion)

#define TS_CS_EMOTION_ID(X) \
	X(1202, version < EPIC_9_6_3) \
	X(2202, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CS_EMOTION, SessionType::GameClient, SessionPacketOrigin::Client);
#undef TS_CS_EMOTION_DEF

