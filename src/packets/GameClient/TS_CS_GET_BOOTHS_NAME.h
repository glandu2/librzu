#ifndef PACKETS_TS_CS_GET_BOOTHS_NAME_H
#define PACKETS_TS_CS_GET_BOOTHS_NAME_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_GET_BOOTHS_NAME_DEF(_) \
	_(count)(int32_t, handles) \
	_(dynarray)(ar_handle_t, handles)

CREATE_PACKET(TS_CS_GET_BOOTHS_NAME, 707);

#endif // PACKETS_TS_CS_GET_BOOTHS_NAME_H
