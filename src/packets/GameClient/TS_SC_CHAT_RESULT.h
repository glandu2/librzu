#ifndef PACKETS_TS_SC_CHAT_RESULT_H
#define PACKETS_TS_SC_CHAT_RESULT_H

#include "PacketDeclaration.h"

#define TS_SC_CHAT_RESULT_DEF(simple_, array_, dynarray_, count_) \
	simple_(char, type) \
	simple_(char, percentage) \
	simple_(int32_t, result) \
	simple_(int32_t, reserved)

CREATE_PACKET(TS_SC_CHAT_RESULT, 24);

#endif // PACKETS_TS_SC_CHAT_RESULT_H