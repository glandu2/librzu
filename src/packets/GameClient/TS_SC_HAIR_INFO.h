#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_HAIR_INFO_DEF(_) \
	_(simple)(ar_handle_t, hPlayer) \
	_(simple)(int32_t, nHairID) \
	_(simple)(int32_t, nHairColorIndex) \
	_(simple)(uint32_t, nHairColorRGB)

#define TS_SC_HAIR_INFO_ID(X) \
	X(220, version < EPIC_9_6_3) \
	X(1220, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_HAIR_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_HAIR_INFO_DEF

