#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_EMOTION_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, emotion)

#define TS_SC_EMOTION_ID(X) \
	X(1201, version < EPIC_9_6_3) \
	X(2201, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_EMOTION, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_EMOTION_DEF

