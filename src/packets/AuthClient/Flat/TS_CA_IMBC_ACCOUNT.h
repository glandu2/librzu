#pragma once

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CA_IMBC_ACCOUNT : public TS_MESSAGE
{
	char account[61];
	unsigned char password[48];
	static const uint16_t packetID = 10012;
};

struct TS_CA_IMBC_ACCOUNT_OLD : public TS_MESSAGE
{
	char account[24];
	unsigned char password[48];
	static const uint16_t packetID = 10012;
};

struct TS_CA_IMBC_ACCOUNT_RSA : public TS_MESSAGE
{
		char account[61];
		unsigned int password_size;
		unsigned char password[64];
		static const int packetID = 10012;

		struct AdditionalInfo
		{
				char type;
				unsigned short size;
		};
};
#pragma pack(pop)

