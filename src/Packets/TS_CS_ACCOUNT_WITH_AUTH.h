#ifndef PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H
#define PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H

#include "PacketDeclaration.h"

#define TS_CS_ACCOUNT_WITH_AUTH_DEF(simple_, array_, dynarray_, count_) \
	array_(def)(char, account, 61) \
	array_(impl)(char, account, 61, version >= EPIC_5_1) \
	array_(impl)(char, account, 19, version <  EPIC_5_1) \
	simple_(uint64_t, one_time_key)

CREATE_PACKET(TS_CS_ACCOUNT_WITH_AUTH, 2005);

#endif // PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H
