#ifndef TS_SC_STATE_RESULT_H
#define TS_SC_STATE_RESULT_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_STATE_RESULT_DEF(_) \
	_(simple)(uint32_t, caster_handle) \
	_(simple)(uint32_t, target_handle) \
	_(simple)(int32_t, code) \
	_(simple)(uint16_t, level) \
	_(simple)(uint16_t, result_type) \
	_(simple)(int32_t, value) \
	_(simple)(int32_t, target_value) \
	_(simple)(bool, final) \
	_(simple)(int32_t, total_amount)

CREATE_PACKET(TS_SC_STATE_RESULT, 406);

#endif // TS_SC_STATE_RESULT_H
