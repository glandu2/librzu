#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CA_SERVER_LIST_DEF(_)
CREATE_PACKET(TS_CA_SERVER_LIST, 10021, SessionType::AuthClient, SessionPacketOrigin::Client);

