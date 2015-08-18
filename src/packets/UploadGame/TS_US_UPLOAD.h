#ifndef PACKETS_TS_US_UPLOAD_H
#define PACKETS_TS_US_UPLOAD_H

#include "PacketBaseMessage.h"
#include "PacketEnums.h"

#pragma pack(push, 1)
struct TS_US_UPLOAD : public TS_MESSAGE_WNA
{
	uint32_t guild_id;
	uint32_t file_size;
	uint8_t filename_length;
	uint8_t type;
	char file_name[0];

	static const uint16_t packetID = 50009;
};
#pragma pack(pop)

#endif // PACKETS_TS_US_UPLOAD_H
