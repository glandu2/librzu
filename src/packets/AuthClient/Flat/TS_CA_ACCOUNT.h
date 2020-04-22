#pragma once

#include "Packet/PacketBaseMessage.h"

/*struct TS_CA_ACCOUNT;
enum TS_CA_ACCOUNT::AdditionalInfoType
{
	BLACKBOX = 0x0,
	MACADDRESS = 0x1,
};*/

#pragma pack(push, 1)
struct TS_CA_ACCOUNT : public TS_MESSAGE
{
	char account[61];
	unsigned char password[61];
	static const uint16_t packetID = 10010;

	struct AdditionalInfo
	{
		int8_t type;
		uint16_t size;
	};
};

struct TS_CA_ACCOUNT_EPIC4 : public TS_MESSAGE
{
	char account[19];
	unsigned char password[32];
	static const uint16_t packetID = 10010;
};

struct TS_CA_ACCOUNT_RSA : public TS_MESSAGE
{
		char account[61];
		unsigned int password_size;
		unsigned char password[61];
		unsigned int dummy[3];
		unsigned int unknown_00000100;
		static const int packetID = 10010;

		struct AdditionalInfo
		{
				char type;
				unsigned short size;
		};
};
#pragma pack(pop)

