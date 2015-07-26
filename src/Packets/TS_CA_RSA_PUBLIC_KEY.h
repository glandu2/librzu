#ifndef PACKETS_TS_CA_RSA_PUBLIC_KEY_H
#define PACKETS_TS_CA_RSA_PUBLIC_KEY_H

#include "PacketDeclaration.h"

#define TS_CA_RSA_PUBLIC_KEY_DEF(simple_, array_, dynarray_, count_) \
	count_(uint32_t, key_size, key) \
	dynarray_(unsigned char, key)

CREATE_PACKET(TS_CA_RSA_PUBLIC_KEY, 71);

#endif // PACKETS_TS_CA_RSA_PUBLIC_KEY_H
