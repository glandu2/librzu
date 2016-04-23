#ifndef PACKETS_TS_CS_PUTON_ITEM_H
#define PACKETS_TS_CS_PUTON_ITEM_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_PUTON_ITEM_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	simple_ (int8_t, position) \
	simple_ (uint32_t, item_handle) \
	simple_ (uint32_t, target_handle)

CREATE_PACKET(TS_CS_PUTON_ITEM, 200);

#endif // PACKETS_TS_CS_PUTON_ITEM_H
