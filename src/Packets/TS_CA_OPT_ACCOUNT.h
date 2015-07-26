#ifndef PACKETS_TS_CA_OPT_ACCOUNT_H
#define PACKETS_TS_CA_OPT_ACCOUNT_H

#include "PacketDeclaration.h"

#define TS_CA_OPT_ACCOUNT_DEF(simple_, array_, dynarray_, count_) \
	array_(char, account, 61) \
	array_(unsigned char, OTP, 48)

CREATE_PACKET(TS_CA_OPT_ACCOUNT, 10011);

#endif // PACKETS_TS_CA_OPT_ACCOUNT_H
