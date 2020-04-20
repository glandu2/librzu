#ifndef PACKETS_TS_SC_REMOVE_PET_INFO_H
#define PACKETS_TS_SC_REMOVE_PET_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_REMOVE_PET_INFO_DEF(_) \
	_(simple)(ar_handle_t, handle)

CREATE_PACKET(TS_SC_REMOVE_PET_INFO, 352);
#undef TS_SC_REMOVE_PET_INFO_DEF

#endif // PACKETS_TS_SC_REMOVE_PET_INFO_H
