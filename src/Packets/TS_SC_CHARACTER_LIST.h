#ifndef PACKETS_TS_SC_CHARACTER_LIST_H
#define PACKETS_TS_SC_CHARACTER_LIST_H

#include "Packets/PacketBaseMessage.h"
#include "Packets/PacketEnums.h"

#pragma pack(push, 1)
struct MODEL_INFO         //sizeof = 140
{
	int sex;
	int race;
	int model_id[5];
	int hair_color_index;
	unsigned int hair_color_rgb;
	unsigned int hide_equip_flag;
	int texture_id;
	int wear_info[24];
};

struct LOBBY_CHARACTER_INFO
{
	MODEL_INFO model_info;
	int level;
	int job;
	int job_level;
	int exp_percentage;
	int hp;
	int mp;
	int permission;
	bool is_banned;
	char name[19];
	unsigned int skin_color;
	char szCreateTime[30];
	char szDeleteTime[30];
	int wear_item_enhance_info[24];
	int wear_item_level_info[24];
	char wear_item_elemental_type[24];
	int wear_appearance_code[24];
};

struct TS_SC_CHARACTER_LIST : public TS_MESSAGE_WNA
{
	unsigned int current_server_time;
	unsigned short last_login_index;
	unsigned short count;
	LOBBY_CHARACTER_INFO characters[0];

	static const int packetID = 2004;
};
#pragma pack(pop)

#endif // PACKETS_TS_SC_CHARACTER_LIST_H
