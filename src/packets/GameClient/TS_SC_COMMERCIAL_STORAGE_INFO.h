#ifndef PACKETS_TS_SC_COMMERCIAL_STORAGE_INFO_H
#define PACKETS_TS_SC_COMMERCIAL_STORAGE_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_COMMERCIAL_STORAGE_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint16_t, total_item_count) \
	simple_ (uint16_t, new_item_count)

CREATE_PACKET(TS_SC_COMMERCIAL_STORAGE_INFO, 10003);

#endif // PACKETS_TS_SC_COMMERCIAL_STORAGE_INFO_H
