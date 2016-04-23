#ifndef PACKETS_TS_CS_USE_ITEM_H
#define PACKETS_TS_CS_USE_ITEM_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_USE_ITEM_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (uint32_t, item_handle) \
	simple_ (uint32_t, target_handle) \
	string_ (szParameter, 32)

CREATE_PACKET(TS_CS_USE_ITEM, 253);

#endif // PACKETS_TS_CS_USE_ITEM_H
