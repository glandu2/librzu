#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_SHOW_SUMMON_NAME_CHANGE_DEF(_) \
	_(simple)(ar_handle_t, handle)

#define TS_SC_SHOW_SUMMON_NAME_CHANGE_ID(X) \
	X(322, version < EPIC_9_6_3) \
	X(1322, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_SHOW_SUMMON_NAME_CHANGE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_SHOW_SUMMON_NAME_CHANGE_DEF

