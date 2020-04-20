#ifndef PACKETS_TS_CS_CHANGE_ITEM_POSITION_H
#define PACKETS_TS_CS_CHANGE_ITEM_POSITION_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_CHANGE_ITEM_POSITION_DEF(_) \
	_(simple)(bool, is_storage) \
	_(simple)(ar_handle_t, item_handle_1) \
	_(simple)(ar_handle_t, item_handle_2)

CREATE_PACKET(TS_CS_CHANGE_ITEM_POSITION, 218);
#undef TS_CS_CHANGE_ITEM_POSITION_DEF

#endif // PACKETS_TS_CS_CHANGE_ITEM_POSITION_H
