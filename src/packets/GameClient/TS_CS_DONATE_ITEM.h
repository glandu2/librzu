#ifndef PACKETS_TS_CS_DONATE_ITEM_H
#define PACKETS_TS_CS_DONATE_ITEM_H

#include "Packet/PacketDeclaration.h"

#define TS_DONATE_ITEM_INFO_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(simple)(int64_t, count)
CREATE_STRUCT(TS_DONATE_ITEM_INFO);

#define TS_CS_DONATE_ITEM_DEF(_) \
	_(simple)(int64_t, gold) \
	_(simple)(int32_t, jp) \
	_(count)(int8_t, items) \
	_(dynarray)(TS_DONATE_ITEM_INFO, items)

CREATE_PACKET(TS_CS_DONATE_ITEM, 258);

#endif // PACKETS_TS_CS_DONATE_ITEM_H
