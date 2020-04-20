#ifndef PACKETS_TS_CS_UNBIND_SKILLCARD_H
#define PACKETS_TS_CS_UNBIND_SKILLCARD_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_UNBIND_SKILLCARD_DEF(_) \
	_(simple)(ar_handle_t, item_handle) \
	_(simple)(ar_handle_t, target_handle)

CREATE_PACKET(TS_CS_UNBIND_SKILLCARD, 285);
#undef TS_CS_UNBIND_SKILLCARD_DEF

#endif // PACKETS_TS_CS_UNBIND_SKILLCARD_H
