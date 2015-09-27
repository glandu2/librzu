#ifndef PACKETS_TS_AC_SERVER_LIST_H
#define PACKETS_TS_AC_SERVER_LIST_H

#include "Packet/PacketDeclaration.h"

#define TS_SERVER_INFO_DEF(simple_, array_, dynarray_, count_, string_) \
	simple_ (uint16_t, server_idx) \
	array_  (char, server_name, 21) \
	simple_ (bool, is_adult_server, version >= EPIC_4_1, false) \
	array_  (char, server_screenshot_url, 256, version >= EPIC_4_1, "about:blank") \
	array_  (char, server_ip, 16) \
	simple_ (int32_t, server_port) \
	simple_ (uint16_t, user_ratio)

CREATE_STRUCT(TS_SERVER_INFO);

#define TS_AC_SERVER_LIST_DEF(simple_, array_, dynarray_, count_, string_) \
	simple_   (uint16_t, last_login_server_idx, version >= EPIC_4_1, 1) \
	count_    (uint16_t, count, servers) \
	dynarray_ (TS_SERVER_INFO, servers)

CREATE_PACKET(TS_AC_SERVER_LIST, 10022);


#endif // PACKETS_TS_AC_SERVER_LIST_H
