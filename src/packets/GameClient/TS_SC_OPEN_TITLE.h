#ifndef PACKETS_TS_SC_OPEN_TITLE_H
#define PACKETS_TS_SC_OPEN_TITLE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_OPEN_TITLE_DEF(_) \
	_(simple)(int32_t, code)

CREATE_PACKET(TS_SC_OPEN_TITLE, 635);

#endif // PACKETS_TS_SC_OPEN_TITLE_H
