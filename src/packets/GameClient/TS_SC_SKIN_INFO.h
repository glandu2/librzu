#ifndef PACKETS_TS_SC_SKIN_INFO_H
#define PACKETS_TS_SC_SKIN_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_SKIN_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(int32_t, nSkinColor)

CREATE_PACKET(TS_SC_SKIN_INFO, 224);

#endif // PACKETS_TS_SC_SKIN_INFO_H
