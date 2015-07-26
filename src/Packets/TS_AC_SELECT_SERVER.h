#ifndef PACKETS_TS_AC_SELECT_SERVER_H
#define PACKETS_TS_AC_SELECT_SERVER_H

#include "PacketDeclaration.h"

#define TS_AC_SELECT_SERVER_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint16_t, result) \
	simple_(int64_t, one_time_key) \
	simple_(uint32_t, pending_time)

CREATE_PACKET(TS_AC_SELECT_SERVER, 10024);

#define TS_AC_SELECT_SERVER_RSA_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint16_t, result) \
	simple_(int32_t, encrypted_data_size) \
	array_ (unsigned char, encrypted_data, 16) \
	simple_(uint32_t, pending_time) \
	simple_(uint32_t, unknown) \
	simple_(uint32_t, unknown2)

CREATE_PACKET(TS_AC_SELECT_SERVER_RSA, 10024);

#endif // PACKETS_TS_AC_SELECT_SERVER_H
