#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMPETE_END_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(simple)(int8_t, end_type) \
	_(string)(winner, 31) \
	_(string)(loser, 31)

#define TS_SC_COMPETE_END_ID(X) \
	X(4506, true)

CREATE_PACKET_VER_ID(TS_SC_COMPETE_END, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_COMPETE_END_DEF

