#ifndef PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H
#define PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_ACCOUNT_WITH_AUTH_DEF(_) \
	_(string)(def)(account, 61) \
	_(string)(impl)(account, 61, version >= EPIC_5_1) \
	_(string)(impl)(account, 19, version <  EPIC_5_1) \
	_(simple)(uint64_t, one_time_key)

CREATE_PACKET(TS_CS_ACCOUNT_WITH_AUTH, 2005);

#endif // PACKETS_TS_CS_ACCOUNT_WITH_AUTH_H
