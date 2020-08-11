#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_AC_AES_KEY_IV_DEF(_) \
	_(count)(uint32_t, data) \
	_(dynarray)(uint8_t, data)

#define TS_AC_AES_KEY_IV_ID(X) \
	X(72, version < EPIC_9_6_3) \
	X(1072, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_AC_AES_KEY_IV, SessionType::AuthClient, SessionPacketOrigin::Server);
#undef TS_AC_AES_KEY_IV_DEF

