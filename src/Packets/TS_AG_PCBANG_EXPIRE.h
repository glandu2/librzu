#ifndef TS_AG_PCBANG_EXPIRE_H
#define TS_AG_PCBANG_EXPIRE_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AG_PCBANG_EXPIRE : public TS_MESSAGE
{
	char account[61];
};
#pragma pack(pop)

#endif // TS_AG_PCBANG_EXPIRE_H
