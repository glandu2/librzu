#ifndef PACKETS_TS_SC_PROPERTY_H
#define PACKETS_TS_SC_PROPERTY_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_PROPERTY_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	simple_ (bool, is_number) \
	string_ (name, 16) \
	simple_ (int64_t, value, is_number == true) \
	count_ (int64_t, string_size, string_value, is_number == false) \
	dynstring_(string_value, true, is_number == false)

CREATE_PACKET(TS_SC_PROPERTY, 507);

#endif // PACKETS_TS_SC_PROPERTY_H
