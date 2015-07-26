#ifndef PACKETS_TS_AC_RESULT_WITH_STRING_H
#define PACKETS_TS_AC_RESULT_WITH_STRING_H

#include "PacketDeclaration.h"

//not in 5.2 client

#define TS_AC_RESULT_WITH_STRING_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint16_t, request_msg_id) \
	simple_(uint16_t, result) \
	simple_(LOGIN_SUCCESS_FLAG, login_flag) \
	count_ (int32_t, strSize, string) \
	dynarray_(char, string)

CREATE_PACKET(TS_AC_RESULT_WITH_STRING, 10002);

#endif // PACKETS_TS_AC_RESULT_WITH_STRING_H
