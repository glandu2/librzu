#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CA_RSA_PUBLIC_KEY_DEF(_) \
	_(count)(int32_t, key) \
	_(dynarray)(uint8_t, key)
CREATE_PACKET(TS_CA_RSA_PUBLIC_KEY, 71);

