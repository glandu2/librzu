#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CA_RSA_PUBLIC_KEY_DEF(_) \
	_(count)(int32_t, key) \
	_(dynarray)(uint8_t, key)

#define TS_CA_RSA_PUBLIC_KEY_ID(X) \
	X(71, version < EPIC_9_6_3) \
	X(1071, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_CA_RSA_PUBLIC_KEY, SessionType::AuthClient, SessionPacketOrigin::Client);
#undef TS_CA_RSA_PUBLIC_KEY_DEF
