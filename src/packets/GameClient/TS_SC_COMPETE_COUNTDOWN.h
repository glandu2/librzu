#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMPETE_COUNTDOWN_DEF(_) \
	_(simple)(int8_t, compete_type) \
	_(string)(competitor, 31) \
	_(simple)(ar_handle_t, handle_competitor)

#define TS_SC_COMPETE_COUNTDOWN_ID(X) \
	X(4504, true)

CREATE_PACKET_VER_ID(TS_SC_COMPETE_COUNTDOWN, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_COMPETE_COUNTDOWN_DEF

