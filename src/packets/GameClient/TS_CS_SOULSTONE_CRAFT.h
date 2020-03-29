#ifndef PACKETS_TS_CS_SOULSTONE_CRAFT_H
#define PACKETS_TS_CS_SOULSTONE_CRAFT_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_SOULSTONE_CRAFT_DEF(_) \
	_(simple)(ar_handle_t, craft_item_handle) \
	_(array)(ar_handle_t, soulstone_handle, 4)

CREATE_PACKET(TS_CS_SOULSTONE_CRAFT, 260);

#endif // PACKETS_TS_CS_SOULSTONE_CRAFT_H
