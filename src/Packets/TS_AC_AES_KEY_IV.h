#ifndef TS_AC_AES_KEY_IV_H
#define TS_AC_AES_KEY_IV_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AC_AES_KEY_IV : public TS_MESSAGE_WNA
{
	int data_size;
	unsigned char rsa_encrypted_data[0];
	static const uint16_t packetID = 72;
};
#pragma pack(pop)

#endif // TS_AC_AES_KEY_IV_H