#ifndef PACKETS_TS_SC_MIX_RESULT_H
#define PACKETS_TS_SC_MIX_RESULT_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_MIX_RESULT_DEF(_) \
	_(count)(uint32_t, handles) \
	_(simple)(int8_t, type) \
	_(dynarray)(uint32_t, handles)

CREATE_PACKET(TS_SC_MIX_RESULT, 257);

#endif // PACKETS_TS_SC_MIX_RESULT_H
