#ifndef PACKETS_TS_AC_AES_KEY_IV_H
#define PACKETS_TS_AC_AES_KEY_IV_H

#include "PacketDeclaration.h"

#define TS_AC_AES_KEY_IV_DEF(simple_, array_, dynarray_, count_) \
	count_   (uint32_t, data_size, rsa_encrypted_data) \
	dynarray_(unsigned char, rsa_encrypted_data)

CREATE_PACKET(TS_AC_AES_KEY_IV, 72);

#endif // PACKETS_TS_AC_AES_KEY_IV_H
