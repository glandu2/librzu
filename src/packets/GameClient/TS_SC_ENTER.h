#ifndef PACKETS_TS_SC_ENTER_H
#define PACKETS_TS_SC_ENTER_H

#include "Packet/PacketBaseMessage.h"
#include "PacketEnums.h"

#pragma pack(push, 1)
struct TS_SC_ENTER : public TS_MESSAGE
{
	char type;
	unsigned int handle;
	float x;
	float y;
	float z;
	char layer;
	char ObjType;

	struct TS_ITEM_PICK_UP_ORDER
	{
		unsigned int drop_time;
		unsigned int hPlayer[3];
		int nPartyID[3];
	};
/*
	struct ItemInfo             //ObjType = 2, type = 2
	{
		int code;
		long long int cnt;
		TS_ENTER::TS_ITEM_PICK_UP_ORDER pick_up_order;
	};

	struct SkillInfo            //ObjType  = 5, type = 2
	{
		unsigned int caster;
		unsigned int start_time;
		int skill_num;
	};

	struct FieldPropInfo        //ObjType = 6, type = 2
	{
		int prop_id;
		float fZOffset;
		float fRotateX;
		float fRotateY;
		float fRotateZ;
		float fScaleX;
		float fScaleY;
		float fScaleZ;
		bool bLockHeight;
		float fLockHeight;
	};
*/
	struct CreatureInfo         //interm size = 38
	{
		unsigned int status;
		float face_direction;
		int hp;
		int max_hp;
		int mp;
		int max_mp;
		int level;
		char race;
		unsigned int skin_color;
		bool is_first_enter;
		int energy;
	};
/*
	struct MonsterInfo         //ObjType = 3, type = 1
	{
		TS_ENTER::CreatureInfo baseclass_0;
		RandomizedInt monster_id;
		bool is_tamed;
	};

	struct SummonInfo          //ObjType = 4, type = 1
	{
		TS_ENTER::CreatureInfo baseclass_0;
		unsigned int master_handle;
		RandomizedInt summon_code;
		char szName[19];
		char enhance;
	};

	struct NPCInfo            //ObjType = 1, type = 1
	{
		TS_ENTER::CreatureInfo baseclass_0;
		RandomizedInt npc_id;
	};
*/
	struct PlayerInfo         //ObjType = 0, type = 0
	{
		CreatureInfo baseclass_0;
		char sex;
		int faceId;
		int faceTextureId;
		int hairId;
		int hairColorIndex;
		unsigned int hairColorRGB;
		unsigned int hideEquipFlag;
		char szName[19];
		unsigned short job_id;
		unsigned int ride_handle;
		int guild_id;
	};
/*
	struct PetInfo             //ObjType = 7, type = 1
	{
		TS_ENTER::CreatureInfo baseclass_0;
		unsigned int master_handle;
		RandomizedInt pet_code;
		char szName[19];
	};*/
	static const int packetID = 3;
};
#pragma pack(pop)

#endif // PACKETS_TS_SC_ENTER_H
