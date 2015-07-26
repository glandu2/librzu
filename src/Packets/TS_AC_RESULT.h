#ifndef PACKETS_TS_AC_RESULT_H
#define PACKETS_TS_AC_RESULT_H

#include "PacketEnums.h"
#include "PacketDeclaration.h"

enum LOGIN_SUCCESS_FLAG : int32_t
{
	LSF_EULA_ACCEPTED = 0x1,
	LSF_ACCOUNT_BLOCK_WARNING = 0x2,
	LSF_DISTRIBUTION_CODE_REQUIRED = 0x4
};

#define TS_AC_RESULT_DEF(simple_, array_, dynarray_, count_) \
	simple_(uint16_t, request_msg_id) \
	simple_(uint16_t, result) \
	simple_(int32_t, login_flag)

CREATE_PACKET(TS_AC_RESULT, 10000);

#endif // PACKETS_TS_AC_RESULT_H
