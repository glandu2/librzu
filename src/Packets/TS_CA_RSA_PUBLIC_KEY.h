#ifndef PACKETS_TS_CA_RSA_PUBLIC_KEY_H
#define PACKETS_TS_CA_RSA_PUBLIC_KEY_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_RSA_PUBLIC_KEY : public TS_MESSAGE_WNA
{
	int key_size;
	unsigned char key[0];
	static const uint16_t packetID = 71;
};
#pragma pack(pop)

#endif // PACKETS_TS_CA_RSA_PUBLIC_KEY_H
