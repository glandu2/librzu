#ifndef TS_SC_STATE_RESULT_H
#define TS_SC_STATE_RESULT_H

#include "Packet/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_SC_STATE_RESULT : public TS_MESSAGE
{
	uint32_t caster_handle;
	uint32_t target_handle;
	int32_t code;
	uint16_t level;
	uint16_t result_type;
	int32_t value;
	int32_t target_value;
	bool final;
	int32_t total_amount;

	static const int packetID = 406;
};
#pragma pack(pop)

#endif // TS_SC_STATE_RESULT_H
