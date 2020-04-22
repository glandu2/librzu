#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_EMOTION_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, emotion)

CREATE_PACKET(TS_SC_EMOTION, 1201);
#undef TS_SC_EMOTION_DEF

