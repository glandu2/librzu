#ifndef PACKETS_TS_CS_SELL_TO_BOOTH_H
#define PACKETS_TS_CS_SELL_TO_BOOTH_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_SELL_TO_BOOTH_DEF(_) \
	_(simple)(ar_handle_t, target) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(int32_t, cnt)

CREATE_PACKET(TS_CS_SELL_TO_BOOTH, 706);

#endif // PACKETS_TS_CS_SELL_TO_BOOTH_H
