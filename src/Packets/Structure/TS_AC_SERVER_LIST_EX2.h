#ifndef TS_AC_SERVER_LIST_EX2_H
#define TS_AC_SERVER_LIST_EX2_H

#include "PacketDeclaration.h"

#define TS_SERVER_INFO_DEF(F_SIMPLE, F_ARRAY, F_DYNARRAY, F_COUNT) \
	F_SIMPLE(uint16_t, server_idx) \
	F_ARRAY(char, server_name, 21) \
	F_SIMPLE(bool, is_adult_server, version >= 040000, false) \
	F_ARRAY(char, server_screenshot_url, 256, version >= 040000, "about:blank") \
	F_ARRAY(char, server_ip, 16) \
	F_SIMPLE(int32_t, server_port) \
	F_SIMPLE(uint16_t, user_ratio)

CREATE_STRUCT(TS_SERVER_INFO2, TS_SERVER_INFO_DEF);

#define TS_AC_SERVER_LIST_DEF(F_SIMPLE, F_ARRAY, F_DYNARRAY, F_COUNT) \
	F_SIMPLE(uint16_t, last_login_server_idx, version >= 040000, 1) \
	F_COUNT(uint16_t, count, servers) \
	F_DYNARRAY(TS_SERVER_INFO2, servers) \
	F_SIMPLE(uint32_t, adata)

CREATE_PACKET(TS_AC_SERVER_LIST2, TS_AC_SERVER_LIST_DEF, 10022);

#endif // TS_AC_SERVER_LIST_EX2_H

