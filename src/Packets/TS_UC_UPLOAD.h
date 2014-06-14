#ifndef TS_UC_UPLOAD_H
#define TS_UC_UPLOAD_H

#include "Packets/PacketBaseMessage.h"

#pragma pack(push, 1)
struct TS_UC_UPLOAD : public TS_MESSAGE
{
	uint16_t result;

	static const uint16_t packetID = 50008;
};
#pragma pack(pop)

#endif // TS_UC_UPLOAD_H
