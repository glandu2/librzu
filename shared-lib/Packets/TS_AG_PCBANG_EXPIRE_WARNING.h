#ifndef TS_AG_PCBANG_EXPIRE_WARNING_H
#define TS_AG_PCBANG_EXPIRE_WARNING_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_PCBANG_EXPIRE_WARNING : public TS_MESSAGE
{
	char account[61];
	uint32_t remain_minutes;
};
#pragma pack(pop)

#endif // TS_AG_PCBANG_EXPIRE_WARNING_H
