#ifndef PACKETS_TS_CS_PUTON_ITEM_SET_H
#define PACKETS_TS_CS_PUTON_ITEM_SET_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_PUTON_ITEM_SET_DEF(_) \
	_(array)(uint32_t, handle, 24)

CREATE_PACKET(TS_CS_PUTON_ITEM_SET, 281);

#endif // PACKETS_TS_CS_PUTON_ITEM_SET_H
