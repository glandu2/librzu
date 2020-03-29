#ifndef PACKETS_TS_CS_USE_ITEM_H
#define PACKETS_TS_CS_USE_ITEM_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_USE_ITEM_DEF(_) \
	_(simple) (ar_handle_t, item_handle) \
	_(simple) (ar_handle_t, target_handle) \
	_(string) (szParameter, 32, version >= EPIC_4_1)

CREATE_PACKET(TS_CS_USE_ITEM, 253);

#endif // PACKETS_TS_CS_USE_ITEM_H
