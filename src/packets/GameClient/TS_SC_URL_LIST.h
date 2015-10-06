#ifndef PACKETS_TS_SC_URL_LIST_H
#define PACKETS_TS_SC_URL_LIST_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_URL_LIST_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	count_(uint16_t, len, url_list) \
	dynstring_(url_list, false)

CREATE_PACKET(TS_SC_URL_LIST, 9001);
	

#endif // PACKETS_TS_SC_URL_LIST_H
