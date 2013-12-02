#ifndef TS_AC_SELECT_SERVER_H
#define TS_AC_SELECT_SERVER_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AC_SELECT_SERVER : public TS_MESSAGE
{
	uint16_t result;
	int64_t one_time_key;
	uint32_t pending_time;
	static const uint16_t packetID = 10024;
};

struct TS_AC_SELECT_SERVER_V2 : public TS_MESSAGE
{
	uint16_t result;
	int32_t encrypted_data_size;
	unsigned char encrypted_data[16];
	uint32_t pending_time;
	uint32_t unknown;
	static const uint16_t packetID = 10024;
};
#pragma pack(pop)

#endif // TS_AC_SELECT_SERVER_H
