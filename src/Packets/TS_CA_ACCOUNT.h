#ifndef PACKETS_TS_CA_ACCOUNT_H
#define PACKETS_TS_CA_ACCOUNT_H

#include "PacketDeclaration.h"

struct TS_CA_ACCOUNT__AdditionalInfo
{
		char type;
		unsigned short size;
};

// if version > EPIC_8_1_1_RSA, this packet contains RSA encrypted password
#define TS_CA_ACCOUNT_DEF(simple_, array_, dynarray_, count_) \
	array_(def)(char, account, 61) \
	array_(impl)(char, account, 61, version >= EPIC_5_1) \
	array_(impl)(char, account, 19, version <  EPIC_5_1) \
	simple_(uint32_t, password_size, version >= EPIC_8_1_1_RSA, (version >= EPIC_5_1)? 61 : 32 ) /* 61 and 32 must match password real field size (in packet) */ \
	array_(def)(unsigned char, password, 61) \
	array_(impl)(unsigned char, password, 61, version >= EPIC_5_1) \
	array_(impl)(unsigned char, password, 32, version <  EPIC_5_1) \
	array_(uint32_t, dummy, 3, version >= EPIC_8_1_1_RSA) \
	simple_(uint32_t, unknown_00000100, version >= EPIC_8_1_1_RSA)

CREATE_PACKET(TS_CA_ACCOUNT, 10010);

#endif // PACKETS_TS_CA_ACCOUNT_H
