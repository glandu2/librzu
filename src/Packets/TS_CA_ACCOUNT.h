#ifndef TS_CA_ACCOUNT_H
#define TS_CA_ACCOUNT_H

#include "PacketBaseMessage.h"

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
	char password[61];
	static const uint16_t packetID = 10010;

	struct AdditionalInfo
	{
		int8_t type;
		uint16_t size;
	};
};

struct TS_CA_ACCOUNT_V2 : public TS_MESSAGE
{
		char account[61];
		unsigned int aes_block_size;
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

#endif // TS_CA_ACCOUNT_H