#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CA_SELECT_SERVER_DEF(_) \
	_(simple)   (uint16_t, server_idx)
CREATE_PACKET(TS_CA_SELECT_SERVER, 10023, SessionType::AuthClient, SessionPacketOrigin::Client);

