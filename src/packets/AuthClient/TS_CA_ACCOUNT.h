#ifndef PACKETS_TS_CA_ACCOUNT_H
#define PACKETS_TS_CA_ACCOUNT_H

#include "Packet/PacketBaseMessage.h"
#include "Packet/PacketDeclaration.h"

enum TS_ADDITIONAL_INFO_TYPE :  int8_t
{
	TAIT_BLACKBOX = 0,
	TAIT_MACADDRESS = 1
};

#define TS_ACCOUNT_ADDITIONAL_INFO_DEF(_) \
	_(simple)(TS_ADDITIONAL_INFO_TYPE, type) \
	_(count)(uint16_t, data) \
	_(dynarray)(uint8_t, data)
CREATE_STRUCT(TS_ACCOUNT_ADDITIONAL_INFO);

#define TS_ACCOUNT_PASSWORD_DES_DEF(_) \
	_(def)(array)(uint8_t, password, 61) \
	_(impl)(array)(uint8_t, password, 61, version >= EPIC_5_1) \
	_(impl)(array)(uint8_t, password, 32, version <  EPIC_5_1)
CREATE_STRUCT(TS_ACCOUNT_PASSWORD_DES);

#define TS_ACCOUNT_PASSWORD_AES_DEF(_) \
	_(simple)(uint32_t, password_size) \
	_(array)(uint8_t, password, 77)
CREATE_STRUCT(TS_ACCOUNT_PASSWORD_AES);

#define TS_CA_ACCOUNT_DEF(_) \
	_(def)(string)(account, 61) \
	_(impl)(string)(account, 61, version >= EPIC_5_1) \
	_(impl)(string)(account, 19, version <  EPIC_5_1) \
	_(simple)(TS_ACCOUNT_PASSWORD_DES, passwordDes, version < EPIC_8_1_1_RSA) \
	_(simple)(TS_ACCOUNT_PASSWORD_AES, passwordAes, version >= EPIC_8_1_1_RSA) \
	_(endarray)(TS_ACCOUNT_ADDITIONAL_INFO, additionalInfos)
CREATE_PACKET(TS_CA_ACCOUNT, 10010);

#pragma pack(push, 1)
struct TS_CA_ACCOUNT_PLAIN : public TS_MESSAGE
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

struct TS_CA_ACCOUNT_EPIC4_PLAIN : public TS_MESSAGE
{
	char account[19];
	unsigned char password[32];
	static const uint16_t packetID = 10010;
};

struct TS_CA_ACCOUNT_RSA_PLAIN : public TS_MESSAGE
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

#endif // PACKETS_TS_CA_ACCOUNT_H
