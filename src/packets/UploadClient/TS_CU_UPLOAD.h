#ifndef PACKETS_TS_CU_UPLOAD_H
#define PACKETS_TS_CU_UPLOAD_H

#include "PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_CU_UPLOAD : public TS_MESSAGE
{
	uint32_t file_length;
	char file_contents[0];

	static const uint16_t packetID = 50007;
};
#pragma pack(pop)

#endif // PACKETS_TS_CU_UPLOAD_H
