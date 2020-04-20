#ifndef PACKETS_TS_CS_CANCEL_ACTION_H
#define PACKETS_TS_CS_CANCEL_ACTION_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_CANCEL_ACTION_DEF(_) \
	_(simple)(ar_handle_t, handle)

CREATE_PACKET(TS_CS_CANCEL_ACTION, 150);
#undef TS_CS_CANCEL_ACTION_DEF

#endif // PACKETS_TS_CS_CANCEL_ACTION_H
