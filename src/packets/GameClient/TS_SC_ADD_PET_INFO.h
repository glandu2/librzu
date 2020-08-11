#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_SC_ADD_PET_INFO_DEF(_) \
	_(simple)(ar_handle_t, cage_handle) \
	_(simple)(ar_handle_t, pet_handle) \
	_(def)(string)(name, 20) \
	  _(impl)(string)(name, 19, version < EPIC_9_6) \
	  _(impl)(string)(name, 20, version >= EPIC_9_6) \
	_(simple)(int32_t, code)

#define TS_SC_ADD_PET_INFO_ID(X) \
	X(351, version < EPIC_9_6_3) \
	X(1351, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_ADD_PET_INFO, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_ADD_PET_INFO_DEF

