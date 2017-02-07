#ifndef PACKETS_TS_SC_STATE_H
#define PACKETS_TS_SC_STATE_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_STATE_DEF(_) \
	_(simple)(uint32_t, handle) \
	_(simple)(uint16_t, state_handle) \
	_(simple)(uint32_t, state_code) \
	_(simple)(uint16_t, state_level) \
	_(simple)(uint32_t, end_time) \
	_(simple)(uint32_t, start_time) \
	_(simple)(int32_t, state_value) \
	_(string)(state_string_value, 32)

CREATE_PACKET(TS_SC_STATE, 505);

#endif // PACKETS_TS_SC_STATE_H
