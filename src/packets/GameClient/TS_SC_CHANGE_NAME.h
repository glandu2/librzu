#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_CHANGE_NAME_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6)

#define TS_SC_CHANGE_NAME_ID(X) \
	X(30, version < EPIC_9_6_3) \
	X(1030, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_CHANGE_NAME, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_CHANGE_NAME_DEF

