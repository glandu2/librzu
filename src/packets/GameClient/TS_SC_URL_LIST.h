#ifndef PACKETS_TS_SC_URL_LIST_H
#define PACKETS_TS_SC_URL_LIST_H

#include "Packet/PacketDeclaration.h"

// len does not include null terminator
// packet does not include null terminator
#define TS_SC_URL_LIST_DEF(_) \
	_(def)(count)(uint32_t, url_list) \
	  _(impl)(count)(uint32_t, url_list, version >= EPIC_9_6_2) \
	  _(impl)(count)(uint16_t, url_list, version < EPIC_9_6_2) \
	_(dynstring)(url_list, false)

CREATE_PACKET(TS_SC_URL_LIST, 9001);
	

#endif // PACKETS_TS_SC_URL_LIST_H
