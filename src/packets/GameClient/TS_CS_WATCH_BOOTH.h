#ifndef PACKETS_TS_CS_WATCH_BOOTH_H
#define PACKETS_TS_CS_WATCH_BOOTH_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_WATCH_BOOTH_DEF(_) \
	_(simple)(ar_handle_t , target)

CREATE_PACKET(TS_CS_WATCH_BOOTH, 702);
#undef TS_CS_WATCH_BOOTH_DEF

#endif // PACKETS_TS_CS_WATCH_BOOTH_H
