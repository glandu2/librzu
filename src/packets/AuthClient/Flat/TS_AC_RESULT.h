#pragma once

#include "Packet/PacketBaseMessage.h"
#include "PacketEnums.h"

#pragma pack(push, 1)
struct TS_AC_RESULT : public TS_MESSAGE
{
	enum LOGIN_SUCCESS_FLAG
	{
		LSF_EULA_ACCEPTED = 0x1,
		LSF_ACCOUNT_BLOCK_WARNING = 0x2,
		LSF_DISTRIBUTION_CODE_REQUIRED = 0x4
	};

	uint16_t request_msg_id;
	uint16_t result;
	int32_t login_flag;
	static const uint16_t packetID = 10000;
};
#pragma pack(pop)

