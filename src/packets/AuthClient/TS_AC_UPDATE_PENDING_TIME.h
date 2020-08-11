#pragma once

#include "Packet/PacketDeclaration.h"

//Not used in 6.1 server ...
#define TS_AC_UPDATE_PENDING_TIME_DEF(_) \
	_(simple)(uint32_t, pending_time)
CREATE_PACKET(TS_AC_UPDATE_PENDING_TIME, 10025, SessionType::AuthClient, SessionPacketOrigin::Server);

