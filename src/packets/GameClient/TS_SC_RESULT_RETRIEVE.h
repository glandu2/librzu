#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_RESULT_RETRIEVE_DEF(_) \
	_(simple)(int8_t, result)

// Since EPIC_7_3
#define TS_SC_RESULT_RETRIEVE_ID(X) \
	X(6005, true)

CREATE_PACKET_VER_ID(TS_SC_RESULT_RETRIEVE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_RESULT_RETRIEVE_DEF

