#ifndef PACKETS_TS_CS_ACCOUNT_H
#define PACKETS_TS_CS_ACCOUNT_H

#include "Packet/PacketDeclaration.h"

#define TS_CS_ACCOUNT_DEF(_) \
	_(string)(account, 19) \
	_(array)(uint8_t, password, 32)

CREATE_PACKET(TS_CS_ACCOUNT, 2000);

#endif // PACKETS_TS_CS_ACCOUNT_H
