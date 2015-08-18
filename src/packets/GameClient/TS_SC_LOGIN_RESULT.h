#ifndef PACKETS_TS_SC_LOGIN_RESULT_H
#define PACKETS_TS_SC_LOGIN_RESULT_H

#include "PacketBaseMessage.h"
#include "PacketEnums.h"

#pragma pack(push, 1)
struct TS_SC_LOGIN_RESULT : public TS_MESSAGE
{
	unsigned short result;
	unsigned int handle;
	float x;
	float y;
	float z;
	char layer;
	float face_direction;
	int region_size;
	int hp;
	int mp;
	int max_hp;
	int max_mp;
	int havoc;
	int max_havoc;
	int sex;
	int race;
	unsigned int skin_color;
	int faceId;
	int hairId;
	char szName[19];
	int cell_size;
	int guild_id;

	static const int packetID = 4;
};
#pragma pack(pop)

#endif // PACKETS_TS_SC_LOGIN_RESULT_H
