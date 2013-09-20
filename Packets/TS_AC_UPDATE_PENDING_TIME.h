#ifndef TS_AC_UPDATE_PENDING_TIME_H
#define TS_AC_UPDATE_PENDING_TIME_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_AC_UPDATE_PENDING_TIME : public TS_MESSAGE //Not used in 6.1 server ...
{
	uint32_t pending_time;
};
#pragma pack(pop)

#endif // TS_AC_UPDATE_PENDING_TIME_H
