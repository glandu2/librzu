#ifndef PACKETS_TS_SC_RESULT_FOSTER_H
#define PACKETS_TS_SC_RESULT_FOSTER_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_RESULT_FOSTER_DEF(_) \
	_(simple)(int8_t, result)

CREATE_PACKET(TS_SC_RESULT_FOSTER, 6003);

#endif // PACKETS_TS_SC_RESULT_FOSTER_H
