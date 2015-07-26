#ifndef PACKETS_TS_CA_IMBC_ACCOUNT_H
#define PACKETS_TS_CA_IMBC_ACCOUNT_H

#include "PacketDeclaration.h"

#define TS_CA_IMBC_ACCOUNT_DEF(simple_, array_, dynarray_, count_) \
	array_(char, account, 61) \
	simple_(uint32_t, password_size, version >= EPIC_8_1_1_RSA, 48) \
	array_(def)(unsigned char, password, 64) \
	array_(impl)(unsigned char, password, 64, version >= EPIC_8_1_1_RSA) \
	array_(impl)(unsigned char, password, 48, version <  EPIC_8_1_1_RSA)

CREATE_PACKET(TS_CA_IMBC_ACCOUNT, 10012);

#endif // PACKETS_TS_CA_IMBC_ACCOUNT_H
