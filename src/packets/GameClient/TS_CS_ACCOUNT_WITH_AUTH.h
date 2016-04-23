#ifndef PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H
#define PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_ACCOUNT_WITH_AUTH_DEF(simple_, array_, dynarray_, count_, string_, dynstring_, pad_) \
	string_(def)(account, 61) \
	string_(impl)(account, 61, version >= EPIC_5_1) \
	string_(impl)(account, 19, version <  EPIC_5_1) \
	simple_(uint64_t, one_time_key)

CREATE_PACKET(TS_CS_ACCOUNT_WITH_AUTH, 2005);

#endif // PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H
