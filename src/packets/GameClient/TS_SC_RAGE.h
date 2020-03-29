#ifndef PACKETS_TS_SC_RAGE_H
#define PACKETS_TS_SC_RAGE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_RAGE_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int16_t, rage)

CREATE_PACKET(TS_SC_RAGE, 506);

#endif // PACKETS_TS_SC_RAGE_H
