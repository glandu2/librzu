#ifndef PACKETS_TS_SC_UNSUMMON_PET_H
#define PACKETS_TS_SC_UNSUMMON_PET_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_UNSUMMON_PET_DEF(_) \
	_(simple)(ar_handle_t, handle)

CREATE_PACKET(TS_SC_UNSUMMON_PET, 350);
#undef TS_SC_UNSUMMON_PET_DEF

#endif // PACKETS_TS_SC_UNSUMMON_PET_H
