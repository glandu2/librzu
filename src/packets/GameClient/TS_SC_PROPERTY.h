#ifndef PACKETS_TS_SC_PROPERTY_H
#define PACKETS_TS_SC_PROPERTY_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_PROPERTY_DEF(_) \
	_(simple) (uint32_t, handle) \
	_(simple) (bool, is_number) \
	_(string) (name, 16) \
	_(simple)(def)(int64_t, value, is_number == true) \
	_(simple)(impl)(int64_t, value, is_number == true && version >= EPIC_5_1) \
	_(simple)(impl)(int32_t, value, is_number == true && version < EPIC_5_1) \
	_(count)(def)(int64_t, string_size, string_value, is_number == false) \
	_(count)(impl)(int64_t, string_size, string_value, is_number == false && version >= EPIC_5_1) \
	_(count)(impl)(int32_t, string_size, string_value, is_number == false && version < EPIC_5_1) \
	_(dynstring)(string_value, true, is_number == false)

CREATE_PACKET(TS_SC_PROPERTY, 507);

#endif // PACKETS_TS_SC_PROPERTY_H
