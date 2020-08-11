#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CA_VERSION_DEF(_) \
	_(string)(szVersion, 20)
CREATE_PACKET(TS_CA_VERSION, 10001, SessionType::AuthClient, SessionPacketOrigin::Client);

