#pragma once

#include "Packet/PacketDeclaration.h"

// No string size field
// String value include null terminator
#define TS_SC_PROPERTY_DEF(_) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (bool, is_number) \
	_(string) (name, 16) \
	_(def)(simple)(int64_t, value) \
	_(impl)(simple)(int64_t, value, version >= EPIC_4_1_1) \
	_(impl)(simple)(int32_t, value, version < EPIC_4_1_1) \
	_(endstring)(string_value, true)

#define TS_SC_PROPERTY_ID(X) \
	X(507, version < EPIC_9_6_3) \
	X(1507, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_PROPERTY, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_PROPERTY_DEF

