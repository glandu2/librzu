#pragma once

#include "Packet/PacketDeclaration.h"

// len does not include null terminator
// packet does not include null terminator
#define TS_SC_URL_LIST_DEF(_) \
	_(def)(count)(uint32_t, url_list) \
	  _(impl)(count)(uint32_t, url_list, version >= EPIC_9_6_2) \
	  _(impl)(count)(uint16_t, url_list, version < EPIC_9_6_2) \
	_(dynstring)(url_list, false)

#define TS_SC_URL_LIST_ID(X) \
	X(9001, version < EPIC_9_6_3) \
	X(8101, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_URL_LIST, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_URL_LIST_DEF
	

