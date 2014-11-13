#ifndef PACKETS_TS_SC_RESULT_H
#define PACKETS_TS_SC_RESULT_H

#include "PacketBaseMessage.h"
#include "PacketEnums.h"

#pragma pack(push, 1)
struct TS_SC_RESULT : public TS_MESSAGE
{
	uint16_t request_msg_id;
	uint16_t result;
	int32_t value;
	static const uint16_t packetID = 0;
};
#pragma pack(pop)

#endif // PACKETS_TS_SC_RESULT_H
