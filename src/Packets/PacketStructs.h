#ifndef PACKETS_PACKETSTRUCTS_H
#define PACKETS_PACKETSTRUCTS_H

#include "PacketBaseMessage.h"
#include "TS_SC_RESULT.h"

#pragma pack(push, 1)

struct TS_CS_REPORT : public TS_MESSAGE
{
	uint16_t report_len;
	static const uint16_t packetID = 8000;
};

struct TS_SC_OPEN_URL : public TS_MESSAGE
{
	bool wait_for_event_scene;
	int32_t width;
	int32_t height;
	uint16_t url_len;

	static const uint16_t packetID = 9000;
};


struct TS_SC_URL_LIST : public TS_MESSAGE
{
	uint16_t url_list_len;

	static const uint16_t packetID = 9001;
};




struct TS_CS_CHECK_ILLEGAL_USER : public TS_MESSAGE
{
	uint32_t log_code;

	static const uint16_t packetID = 57;
};



struct TS_SC_XTRAP_CHECK : public TS_MESSAGE
{
	int8_t pCheckBuffer[128];

	static const uint16_t packetID = 58;
};


struct TS_CS_XTRAP_CHECK : public TS_MESSAGE
{
	int8_t pCheckBuffer[128];

	static const uint16_t packetID = 59;
};


struct TS_CS_REQUEST : public TS_MESSAGE
{
	int8_t t;
};



struct TS_SC_CHANGE_NAME : public TS_MESSAGE
{
	uint32_t handle;
	char name[19];

	static const uint16_t packetID = 30;
};




struct TS_TIMESYNC : public TS_MESSAGE
{
	uint32_t time;
	static const uint16_t packetID = 2;
};

struct TS_SET_TIME : public TS_MESSAGE
{
	int32_t gap;

	static const uint16_t packetID = 10;
};

struct TS_CS_RESURRECTION : public TS_MESSAGE
{
	uint32_t handle;
	int8_t type;

	static const uint16_t packetID = 513;
};

struct TS_SC_STATE : public TS_MESSAGE
{
	uint32_t handle;
	uint16_t state_handle;
	uint32_t state_code;
	uint16_t state_level;
	uint32_t end_time;
	uint32_t start_time;
	int32_t state_value;
	char state_string_value[32];

	static const uint16_t packetID = 505;
};




struct TS_SC_PROPERTY : public TS_MESSAGE
{
	uint32_t handle;
	bool is_number;
	char name[16];
	int64_t value;

	static const uint16_t packetID = 507;
};



struct TS_CS_SET_PROPERTY : public TS_MESSAGE
{
	char name[16];

	static const uint16_t packetID = 508;
};



struct TS_SC_HAVOC : public TS_MESSAGE
{
	uint32_t handle;
	int16_t havoc;
};




struct TS_SC_HPMP : public TS_MESSAGE
{
	uint32_t handle;
	int32_t add_hp;
	int32_t hp;
	int32_t max_hp;
	int32_t add_mp;
	int32_t mp;
	int32_t max_mp;
	bool need_to_display;

	static const uint16_t packetID = 509;
};




struct TS_SC_SP : public TS_MESSAGE
{
	uint32_t handle;
	int16_t sp;
	int16_t max_sp;

	static const uint16_t packetID = 514;
};




struct TS_SC_ENERGY : public TS_MESSAGE
{
	uint32_t handle;
	int16_t energy;

	static const uint16_t packetID = 515;
};




struct TS_SC_REGEN_HPMP : public TS_MESSAGE
{
	uint32_t handle;
	int32_t hp_regen;
	int32_t mp_regen;
	int32_t hp;
	int32_t mp;

	static const uint16_t packetID = 516;
};









struct TS_SC_TAMING_INFO : public TS_MESSAGE
{
	int8_t mode;
	uint32_t tamer_handle;
	uint32_t target_handle;

	static const uint16_t packetID = 310;
};



struct TS_WEAR_INFO : public TS_MESSAGE
{
	uint32_t handle;
	int32_t itemCode[24];
	int32_t itemEnhance[24];
	int32_t itemLevel[24];
	int8_t elemental_effect_type[24];
	int32_t appearance_code[24];

	static const uint16_t packetID = 202;
};




struct TS_SC_ITEM_WEAR_INFO : public TS_MESSAGE
{
	uint32_t item_handle;
	int16_t wear_position;
	uint32_t target_handle;
	int32_t enhance;
	int8_t elemental_effect_type;
	int32_t appearance_code;

	static const uint16_t packetID = 287;
};












struct TS_SC_MOUNT_SUMMON : public TS_MESSAGE
{
	uint32_t handle;
	uint32_t summon_handle;
	float x;
	float y;
	bool success;

	static const uint16_t packetID = 320;
};




struct TS_SC_UNMOUNT_SUMMON : public TS_MESSAGE
{
	uint32_t handle;
	uint32_t summon_handle;
	int8_t flag;

	static const uint16_t packetID = 321;
};




struct TS_SC_SHOW_SUMMON_NAME_CHANGE : public TS_MESSAGE
{
	uint32_t handle;

	static const uint16_t packetID = 322;
};



struct TS_CS_CHANGE_SUMMON_NAME : public TS_MESSAGE
{
	char szName[19];

	static const uint16_t packetID = 323;
};


struct TS_CS_GET_SUMMON_SETUP_INFO : public TS_MESSAGE
{
	bool show_dialog;

	static const uint16_t packetID = 324;
};



struct TS_SC_UNSUMMON_PET : public TS_MESSAGE
{
	uint32_t handle;

	static const uint16_t packetID = 350;
};




struct TS_SC_ADD_PET_INFO : public TS_MESSAGE
{
	uint32_t cage_handle;
	uint32_t pet_handle;
	char name[19];
	int32_t code;

	static const uint16_t packetID = 351;
};




struct TS_SC_REMOVE_PET_INFO : public TS_MESSAGE
{
	uint32_t handle;

	static const uint16_t packetID = 352;
};




struct TS_SC_SHOW_SET_PET_NAME : public TS_MESSAGE
{
	uint32_t handle;

	static const uint16_t packetID = 353;
};




struct TS_CS_SET_PET_NAME : public TS_MESSAGE
{
	uint32_t handle;
	char name[19];

	static const uint16_t packetID = 354;
};










struct TS_SC_WARP : public TS_MESSAGE
{
	float x;
	float y;
	float z;
	int8_t layer;

	static const uint16_t packetID = 12;
};
















struct TS_ATTACK_REQUEST : public TS_MESSAGE
{
	uint32_t handle;
	uint32_t target_handle;

	static const uint16_t packetID = 100;
};




struct TS_SC_STATE_RESULT : public TS_MESSAGE
{
	uint32_t caster_handle;
	uint32_t target_handle;
	int32_t code;
	uint16_t level;
	uint16_t result_type;
	int32_t value;
	int32_t target_value;
	bool final;
	int32_t total_amount;

	static const uint16_t packetID = 406;
};




struct TS_ATTACK_EVENT : public TS_MESSAGE_WNA
{
	uint32_t attacker_handle;
	uint32_t target_handle;
	uint16_t attack_speed;
	uint16_t attack_delay;
	int8_t attack_action;
	int8_t attack_flag;
	int8_t count;

	struct ATTACK_INFO
	{
		int32_t damage;
		int32_t mp_damage;
		int8_t flag;
		int32_t elemental_damage[7];
		int32_t target_hp;
		int32_t target_mp;
		int32_t attacker_damage;
		int32_t attacker_mp_damage;
		int32_t attacker_hp;
		int32_t attacker_mp;
	} attacks[0];

	static const uint16_t packetID = 101;
};




struct TS_SC_KNOCK_BACK : public TS_MESSAGE
{
	uint32_t handle;
	float x;
	float y;
	float z;
	int8_t layer;
	float face;
	uint32_t next_movable_time;
};




struct TS_SC_CANT_ATTACK : public TS_MESSAGE
{
	uint32_t attacker_handle;
	uint32_t target_handle;
	int32_t reason;

	static const uint16_t packetID = 102;
};




struct TS_CS_BIND_SKILLCARD : public TS_MESSAGE
{
	uint32_t item_handle;
	uint32_t target_handle;

	static const uint16_t packetID = 284;
};




struct TS_CS_UNBIND_SKILLCARD : public TS_MESSAGE
{
	uint32_t item_handle;
	uint32_t target_handle;

	static const uint16_t packetID = 285;
};




struct TS_SC_SKILLCARD_INFO : public TS_MESSAGE
{
	uint32_t item_handle;
	uint32_t target_handle;

	static const uint16_t packetID = 286;
};



struct TS_CS_PUTON_ITEM : public TS_MESSAGE
{
	int8_t position;
	uint32_t item_handle;
	uint32_t target_handle;

	static const uint16_t packetID = 200;
};



struct TS_CS_PUTON_ITEM_SET : public TS_MESSAGE
{
	uint32_t handle[24];

	static const uint16_t packetID = 281;
};



struct TS_CS_PUTON_CARD : public TS_MESSAGE
{
	int8_t position;
	uint32_t item_handle;

	static const uint16_t packetID = 214;
};


struct TS_CS_PUTOFF_CARD : public TS_MESSAGE
{
	int8_t position;

	static const uint16_t packetID = 215;
};



struct TS_SC_BELT_SLOT_INFO : public TS_MESSAGE
{
	uint32_t handle[6];

	static const uint16_t packetID = 216;
};



struct TS_CS_PUTOFF_ITEM : public TS_MESSAGE
{
	int8_t position;
	uint32_t target_handle;

	static const uint16_t packetID = 201;
};



struct TS_CS_DROP_ITEM : public TS_MESSAGE
{
	uint32_t item_handle;
	int32_t count;

	static const uint16_t packetID = 203;
};




struct TS_CS_TAKE_ITEM : public TS_MESSAGE
{
	uint32_t taker_handle;
	uint32_t item_handle;

	static const uint16_t packetID = 204;
};




struct TS_SC_TAKE_ITEM_RESULT : public TS_MESSAGE
{
	uint32_t item_handle;
	uint32_t item_taker;

	static const uint16_t packetID = 210;
};




struct TS_SC_DROP_RESULT : public TS_MESSAGE
{
	uint32_t item_handle;
	bool isAccepted;

	static const uint16_t packetID = 205;
};




struct TS_CS_ERASE_ITEM : public TS_MESSAGE_WNA
{
	int8_t item_count;

	struct EraseItemInfo
	{
		uint32_t item_handle;
		int64_t count;
		int8_t is_in_storage;
	} items[0];

	static const uint16_t packetID = 208;
};




struct TS_SC_ERASE_ITEM : public TS_MESSAGE_WNA
{
	int8_t item_count;

	struct EraseItemInfo
	{
		uint32_t item_handle;
		int64_t count;
	} items[0];
};





struct TS_ITEM_BASE_INFO
{
	uint32_t handle;
	int32_t Code;
	int64_t uid;
	int64_t count;
	int32_t ethereal_durability;
	uint32_t endurance;
	int8_t enhance;
	int8_t level;
	int32_t Flag;
	int32_t socket[4];
	int32_t awakening_id[5];
	int32_t awakening_values[5];
	int32_t remain_time;
	int8_t elemental_effect_type;
	int32_t elemental_effect_remain_time;
	int32_t elemental_effect_attack_point;
	int32_t elemental_effect_magic_point;
	int32_t appearance_code;
};




struct TS_ITEM_INFO : public TS_ITEM_BASE_INFO
{
	int16_t wear_position;
	uint32_t own_summon_handle;
	int32_t index;
};




struct TS_SC_INVENTORY : public TS_MESSAGE_WNA
{
	uint16_t count;

	TS_ITEM_INFO items[0];

	static const uint16_t packetID = 207;
};



struct TS_SC_OPEN_STORAGE : public TS_MESSAGE
{
	int32_t maxStorageItemCount;

	static const uint16_t packetID = 211;
};



struct TS_SC_COMMERCIAL_STORAGE_INFO : public TS_MESSAGE
{
	uint16_t total_item_count;
	uint16_t new_item_count;

	static const uint16_t packetID = 10003;
};




struct TS_SC_COMMERCIAL_STORAGE_LIST : public TS_MESSAGE_WNA
{
	uint16_t count;

	struct CommercialItemInfo
	{
		uint32_t commercial_item_uid;
		int32_t code;
		uint16_t count;
	} items[0];

	static const uint16_t packetID = 10004;
};




struct TS_CS_TAKEOUT_COMMERCIAL_ITEM : public TS_MESSAGE
{
	uint32_t commercial_item_uid;
	uint16_t count;

	static const uint16_t packetID = 10005;
};




struct TS_CS_GET_REGION_INFO : public TS_MESSAGE
{
	float x;
	float y;

	static const uint16_t packetID = 550;
};



struct TS_SC_SHOW_CREATE_GUILD : public TS_MESSAGE
{

	static const uint16_t packetID = 650;
};


struct TS_SC_SHOW_CREATE_ALLIANCE : public TS_MESSAGE
{

	static const uint16_t packetID = 660;
};



struct TS_SC_OPEN_GUILD_WINDOW : public TS_MESSAGE
{
	int32_t client_id;
	int32_t account_id;
	int32_t one_time_password;
	char raw_server_name[32];

	static const uint16_t packetID = 651;
};




struct TS_SC_UPDATE_GUILD_ICON : public TS_MESSAGE
{
	int32_t client_id;
	int32_t account_id;
	int32_t one_time_password;
	char raw_server_name[32];
};




struct TS_SC_UPDATE_GUILD_BANNER : public TS_MESSAGE
{
	int32_t client_id;
	int32_t account_id;
	int32_t one_time_password;
	char raw_server_name[32];
};



struct TS_CS_CHANGE_ITEM_POSITION : public TS_MESSAGE
{
	bool is_storage;
	uint32_t item_handle_1;
	uint32_t item_handle_2;

	static const uint16_t packetID = 218;
};



struct TS_CS_STORAGE : public TS_MESSAGE
{
	uint32_t item_handle;
	int8_t mode;
	int64_t count;

	static const uint16_t packetID = 212;
};




struct TS_SC_ITEM_COOL_TIME : public TS_MESSAGE
{
	uint32_t cool_time[40];

	static const uint16_t packetID = 217;
};




struct TS_SC_GET_CHAOS : public TS_MESSAGE
{
	uint32_t hPlayer;
	uint32_t hCorpse;
	int32_t nChaos;
	int8_t nBonusType;
	int8_t nBonusPercent;
	int32_t nBonus;

	static const uint16_t packetID = 213;
};



struct TS_CS_ARRANGE_ITEM : public TS_MESSAGE
{
	bool bIsStorage;

	static const uint16_t packetID = 219;
};



struct TS_SC_HAIR_INFO : public TS_MESSAGE
{
	uint32_t hPlayer;
	int32_t nHairID;
	int32_t nHairColorIndex;
	uint32_t nHairColorRGB;
};




struct TS_CS_HIDE_EQUIP_INFO : public TS_MESSAGE
{
	uint32_t nHideEquipFlag;
};




struct TS_SC_HIDE_EQUIP_INFO : public TS_MESSAGE
{
	uint32_t hPlayer;
	uint32_t nHideEquipFlag;
};



struct TS_CS_SWAP_EQUIP : public TS_MESSAGE
{
};



struct TS_CS_MIX : public TS_MESSAGE_WNA
{
	struct MIX_INFO
	{
		uint32_t handle;
		uint16_t count;
	};

	MIX_INFO main_item;
	uint16_t count;
	MIX_INFO mix_infos[0];

	static const uint16_t packetID = 256;
};




struct TS_SC_MIX_RESULT : public TS_MESSAGE_WNA
{
	uint32_t count;
	unsigned int handles[0]; //Not sure about type

	static const uint16_t packetID = 257;
};




struct TS_SC_CHAT_LOCAL : public TS_MESSAGE
{
	uint32_t handle;
	int8_t len;
	int8_t type;
	static const uint16_t packetID = 21;
};




struct TS_SC_CHAT_RESULT : public TS_MESSAGE
{
	int8_t type;
	int8_t percentage;
	int32_t result;
	int32_t reserved;
	static const uint16_t packetID = 24;
};




struct TS_SC_CHAT : public TS_MESSAGE
{
	char szSender[21];
	uint16_t len;
	uint8_t type;
	static const uint16_t packetID = 22;
};



struct TS_CS_CHAT_REQUEST : public TS_MESSAGE
{
	char szTarget[21];
	int8_t request_id;
	uint8_t len;
	uint8_t type;
	static const uint16_t packetID = 20;
};

struct CreatureStat
{
  int16_t stat_id;
  int16_t strength;
  int16_t vital;
  int16_t dexterity;
  int16_t agility;
  int16_t intelligence;
  int16_t mentality;
  int16_t luck;
};

struct CreatureAttribute
{
  int16_t nCritical;
  int16_t nCriticalPower;
  int16_t nAttackPointRight;
  int16_t nAttackPointLeft;
  int16_t nDefence;
  int16_t nBlockDefence;
  int16_t nMagicPoint;
  int16_t nMagicDefence;
  int16_t nAccuracyRight;
  int16_t nAccuracyLeft;
  int16_t nMagicAccuracy;
  int16_t nAvoid;
  int16_t nMagicAvoid;
  int16_t nBlockChance;
  int16_t nMoveSpeed;
  int16_t nAttackSpeed;
  int16_t nAttackRange;
  int32_t nMaxWeight;
  int16_t nCastingSpeed;
  int16_t nCoolTimeSpeed;
  int16_t nItemChance;
  int16_t nHPRegenPercentage;
  int16_t nHPRegenPoint;
  int16_t nMPRegenPercentage;
  int16_t nMPRegenPoint;
  int16_t nPerfectBlock;
  int16_t nMagicalDefIgnore;
  int16_t nMagicalDefIgnoreRatio;
  int16_t nPhysicalDefIgnore;
  int16_t nPhysicalDefIgnoreRatio;
  int16_t nMagicalPenetration;
  int16_t nMagicalPenetrationRatio;
  int16_t nPhysicalPenetration;
  int16_t nPhysicalPenetrationRatio;
};



struct TS_SC_STAT_INFO : public TS_MESSAGE
{
	uint32_t handle;
	CreatureStat stat;
	CreatureAttribute attribute;
	int8_t type;	//0: base stats, 1: buffs stats

	static const uint16_t packetID = 1000;
};




struct TS_SC_REGEN_INFO : public TS_MESSAGE
{
	uint32_t handle;
	int16_t hp_regen_percentage;
	int32_t hp_regen_point;
	int16_t mp_regen_percentage;
	int32_t mp_regen_point;
};








struct TS_CS_SKILL : public TS_MESSAGE
{
	uint16_t skill_id;
	uint32_t caster;
	uint32_t target;
	float x;
	float y;
	float z;
	int8_t layer;
	int8_t skill_level;

	static const uint16_t packetID = 400;
};




struct TS_SC_SKILL : public TS_MESSAGE
{
	struct CancelType
	{
		char dummy[1];
	};

	struct CompleteType
	{
		char dummy[1];
	};
	struct CastType
	{
		uint32_t tm;
		uint16_t nErrorCode;
	};

	struct FireType
	{
		bool bMultiple;
		float range;
		int8_t target_count;
		int8_t fire_count;
		uint16_t count;
	};

	uint16_t skill_id;
	int8_t skill_level;
	uint32_t caster;
	uint32_t target;
	float x;
	float y;
	float z;
	int8_t layer;
	int8_t type;
	int32_t hp_cost;
	int32_t mp_cost;
	int32_t caster_hp;
	int32_t caster_mp;
	union {
		TS_SC_SKILL::CancelType cancel;
		TS_SC_SKILL::CompleteType complete;
		TS_SC_SKILL::CastType cast;
		TS_SC_SKILL::FireType fire;
	};

	static const uint16_t packetID = 401;
};




struct TS_SC_AURA : public TS_MESSAGE
{
	uint32_t caster;
	uint16_t skill_id;
	bool status;

	static const uint16_t packetID = 407;
};




struct TS_CS_REQUEST_REMOVE_STATE : public TS_MESSAGE
{
	uint32_t target;
	int32_t state_code;

	static const uint16_t packetID = 408;
};




struct TS_CS_SUMMON_CARD_SKILL_LIST : public TS_MESSAGE
{
	uint32_t item_handle;
};




struct TS_SC_SKILL_LEVEL_LIST : public TS_MESSAGE_WNA
{
	uint16_t count;

	struct SkillLevelInfo
	{
		int32_t skill_id;
		int8_t skill_level;
	} skills[0];
};




struct TS_SC_SKILL_EFFECT : public TS_MESSAGE
{
	uint32_t caster;
	uint32_t target;
	uint16_t skill_id;
	int8_t skill_level;
	int32_t var1;
	int8_t var2;
	int8_t var3;
};




struct TS_SC_GOLD_UPDATE : public TS_MESSAGE
{
	int64_t gold;
	int32_t chaos;

	static const uint16_t packetID = 1001;
};




struct TS_SC_EXP_UPDATE : public TS_MESSAGE
{
	uint32_t handle;
	int64_t exp;
	int64_t jp;

	static const uint16_t packetID = 1003;
};




struct TS_SC_BONUS_EXP_JP : public TS_MESSAGE_WNA
{
	uint32_t handle;
	uint16_t count;

	struct BONUS_INFO
	{
		int32_t type;
		int32_t rate;
		int64_t exp;
		int32_t jp;
	} bonuses[0];

};




struct TS_SC_LEVEL_UPDATE : public TS_MESSAGE
{
	uint32_t handle;
	int32_t level;
	int32_t job_level;

	static const uint16_t packetID = 1002;
};













struct TS_SC_DIALOG : public TS_MESSAGE
{
	int32_t type;
	uint32_t npc_handle;
	uint16_t title_length;
	uint16_t text_length;
	uint16_t menu_length;

	static const uint16_t packetID = 3000;
};




struct TS_CS_DIALOG : public TS_MESSAGE
{
	uint16_t trigger_length;

	static const uint16_t packetID = 3001;
};




struct TS_CS_CONTACT : public TS_MESSAGE
{
	uint32_t handle;

	static const uint16_t packetID = 3002;
};




struct TS_SC_SHOW_WINDOW : public TS_MESSAGE
{
	uint16_t window_length;
	uint16_t argument_length;
	uint16_t trigger_length;

	static const uint16_t packetID = 3003;
};




struct TS_SC_NPC_TRADE_INFO : public TS_MESSAGE
{
	bool is_sell;
	int32_t code;
	int64_t count;
	int64_t price;
	int32_t huntaholic_point;
	uint32_t target;

	static const uint16_t packetID = 240;
};




struct TS_SC_MARKET : public TS_MESSAGE_WNA
{
	uint32_t npc_handle;
	uint16_t item_count;

	struct ItemInfo
	{
		int32_t code;
		int64_t price;
		int32_t huntaholic_point;
	} items[0];

	static const uint16_t packetID = 250;
};




struct TS_CS_BUY_ITEM : public TS_MESSAGE
{
	int32_t item_code;
	uint16_t buy_count;

	static const uint16_t packetID = 251;
};




struct TS_CS_SELL_ITEM : public TS_MESSAGE
{
	uint32_t handle;
	uint16_t sell_count;

	static const uint16_t packetID = 252;
};




struct TS_CS_DONATE_ITEM : public TS_MESSAGE_WNA
{
	int64_t gold;
	int32_t jp;
	int8_t item_count;

	struct DonateItemInfo
	{
		uint32_t handle;
		int64_t count;
	} items[0];

	static const uint16_t packetID = 258;
};




struct TS_CS_DONATE_REWARD : public TS_MESSAGE_WNA
{
	int8_t reward_count;

	struct RewardInfo
	{
		int8_t reward_type;
		uint16_t count;
	} rewards[0];

	static const uint16_t packetID = 259;
};



struct TS_SC_SHOW_SOULSTONE_CRAFT_WINDOW : public TS_MESSAGE
{

	static const uint16_t packetID = 259;
};



struct TS_CS_SOULSTONE_CRAFT : public TS_MESSAGE
{
	uint32_t craft_item_handle;
	uint32_t soulstone_handle[4];

	static const uint16_t packetID = 260;
};



struct TS_SC_SHOW_SOULSTONE_REPAIR_WINDOW : public TS_MESSAGE
{

	static const uint16_t packetID = 261;
};



struct TS_CS_REPAIR_SOULSTONE : public TS_MESSAGE
{
	uint32_t item_handle[6];

	static const uint16_t packetID = 262;
};




struct TS_CS_TRANSMIT_ETHEREAL_DURABILITY : public TS_MESSAGE
{
	uint32_t handle;
};




struct TS_CS_TRANSMIT_ETHEREAL_DURABILITY_TO_EQUIPMENT : public TS_MESSAGE
{
	float rate;
};




struct TS_CS_USE_ITEM : public TS_MESSAGE
{
	uint32_t item_handle;
	uint32_t target_handle;
	char szParameter[32];

	static const uint16_t packetID = 253;
};




struct TS_SC_USE_ITEM_RESULT : public TS_MESSAGE
{
	uint32_t item_handle;
	uint32_t target_handle;

	static const uint16_t packetID = 283;
};




struct TS_SC_DESTROY_ITEM : public TS_MESSAGE
{
	uint32_t item_handle;

	static const uint16_t packetID = 254;
};




struct TS_SC_UPDATE_ITEM_COUNT : public TS_MESSAGE
{
	uint32_t item_handle;
	int64_t count;

	static const uint16_t packetID = 255;
};




struct TS_TRADE : public TS_MESSAGE
{
	uint32_t target_player;
	int8_t mode;
	TS_ITEM_INFO item_info;

	static const uint16_t packetID = 280;
};




struct TS_SC_SKILL_LIST : public TS_MESSAGE_WNA
{
	uint32_t target;
	uint16_t count;
	int8_t modification_type;

	struct SkillInfo
	{
		int32_t skill_id;
		int8_t base_skill_level;
		int8_t current_skill_level;
		uint32_t total_cool_time;
		uint32_t remain_cool_time;
	} skills[0];

	static const uint16_t packetID = 403;
};




struct TS_SC_ADDED_SKILL_LIST : public TS_MESSAGE_WNA
{
	uint32_t target;
	uint16_t count;

	struct AddedSkillInfo
	{
		int32_t skill_id;
		bool restricted_to_type;
		int8_t added_skill_level;
	} skills[0];

	static const uint16_t packetID = 404;
};




struct TS_CS_LEARN_SKILL : public TS_MESSAGE
{
	uint32_t target;
	int32_t skill_id;
	int16_t skill_level;

	static const uint16_t packetID = 402;
};




struct TS_CS_JOB_LEVEL_UP : public TS_MESSAGE
{
	uint32_t target;

	static const uint16_t packetID = 410;
};




struct TS_SC_SUMMON_EVOLUTION : public TS_MESSAGE
{
	uint32_t card_handle;
	uint32_t summon_handle;
	char name[19];
	int32_t code;

	static const uint16_t packetID = 307;
};




struct TS_SC_ADD_SUMMON_INFO : public TS_MESSAGE
{
	uint32_t card_handle;
	uint32_t summon_handle;
	char name[19];
	int32_t code;
	int32_t level;
	int32_t sp;

	static const uint16_t packetID = 301;
};




struct TS_SC_REMOVE_SUMMON_INFO : public TS_MESSAGE
{
	uint32_t card_handle;

	static const uint16_t packetID = 302;
};



struct TS_EQUIP_SUMMON : public TS_MESSAGE
{
	bool open_dialog;
	uint32_t card_handle[6];
	static const uint16_t packetID = 303;
};


struct TS_CS_SUMMON : public TS_MESSAGE
{
	int8_t is_summon;
	uint32_t card_handle;
};



struct TS_SC_UNSUMMON_NOTICE : public TS_MESSAGE
{
	uint32_t summon_handle;
	uint32_t unsummon_duration;

	static const uint16_t packetID = 306;
};




struct TS_SC_UNSUMMON : public TS_MESSAGE
{
	uint32_t summon_handle;

	static const uint16_t packetID = 305;
};




struct TS_SC_ITEM_DROP_INFO : public TS_MESSAGE
{
	uint32_t monster_handle;
	uint32_t item_handle;

	static const uint16_t packetID = 282;
};




struct TS_SC_QUEST_LIST : public TS_MESSAGE_WNA
{
	uint16_t count_active;
	uint16_t count_pending;

	struct TS_QUEST_INFO
	{
		int32_t code;
		int32_t nStartID;
		int32_t nValue[6];
		int32_t nStatus[6];
		int8_t nProgress;
		uint32_t nTimeLimit;
	} quests[0];

	struct TS_PENDING_QUEST_INFO
	{
		int32_t code;
		int32_t nStartID;
	}pending_quest[0];

	static const uint16_t packetID = 600;
};




struct TS_SC_QUEST_STATUS : public TS_MESSAGE
{
	int32_t code;
	int32_t nStatus[6];
	int8_t nProgress;
	uint32_t nTimeLimit;

	static const uint16_t packetID = 601;
};




struct TS_SC_QUEST_INFOMATION : public TS_MESSAGE
{
	int32_t code;
	enum QUEST_PROGRESS {
	  IS_STARTABLE = 0x0,
	  IS_IN_PROGRESS = 0x1,
	  IS_FINISHABLE = 0x2,
	} nProgress;
	uint16_t trigger_length;
};




struct TS_CS_DROP_QUEST : public TS_MESSAGE
{
	int32_t code;

	static const uint16_t packetID = 603;
};




struct TS_CS_QUEST_INFO : public TS_MESSAGE
{
	int32_t code;
};




struct TS_CS_END_QUEST : public TS_MESSAGE
{
	int32_t code;
	int8_t nOptionalReward;
};














struct TS_CS_START_BOOTH : public TS_MESSAGE
{
	char name[49];
	int8_t type;
	uint16_t cnt;

	struct TS_BOOTH_OPEN_ITEM_INFO
	{
		uint32_t item_handle;
		int32_t cnt;
		int64_t gold;
	};

	static const uint16_t packetID = 700;
};



struct TS_CS_STOP_BOOTH : public TS_MESSAGE
{

	static const uint16_t packetID = 701;
};



struct TS_CS_WATCH_BOOTH : public TS_MESSAGE
{
	uint32_t target;

	static const uint16_t packetID = 702;
};




struct TS_SC_WATCH_BOOTH : public TS_MESSAGE
{
	uint32_t target;
	int8_t type;
	uint16_t cnt;

	struct TS_BOOTH_ITEM_INFO : public TS_ITEM_BASE_INFO
	{
		int64_t gold;
	};

	static const uint16_t packetID = 703;
};




struct TS_CS_STOP_WATCH_BOOTH : public TS_MESSAGE
{
	uint32_t target;

	static const uint16_t packetID = 704;
};




struct TS_CS_BUY_FROM_BOOTH : public TS_MESSAGE
{
	uint32_t target;
	int16_t cnt;

	static const uint16_t packetID = 705;
};




struct TS_SC_BOOTH_TRADE_INFO : public TS_MESSAGE
{
	uint32_t target;
	bool is_sell;
	int16_t cnt;
};




struct TS_CS_SELL_TO_BOOTH : public TS_MESSAGE
{
	uint32_t target;
	uint32_t item_handle;
	int32_t cnt;

	static const uint16_t packetID = 706;
};




struct TS_CS_GET_BOOTHS_NAME : public TS_MESSAGE
{
	uint32_t cnt;

	static const uint16_t packetID = 707;
};




struct TS_SC_GET_BOOTHS_NAME : public TS_MESSAGE
{
	uint32_t cnt;

	struct TS_BOOTH_NAME
	{
		uint32_t handle;
		char name[49];
	};

	static const uint16_t packetID = 708;
};




struct TS_SC_BOOTH_CLOSED : public TS_MESSAGE
{
	uint32_t target;

	static const uint16_t packetID = 709;
};



struct TS_CS_CHECK_BOOTH_STARTABLE : public TS_MESSAGE
{

	static const uint16_t packetID = 711;
};


struct TS_CS_TURN_ON_PK_MODE : public TS_MESSAGE
{

	static const uint16_t packetID = 800;
};


struct TS_CS_TURN_OFF_PK_MODE : public TS_MESSAGE
{

	static const uint16_t packetID = 801;
};









struct TS_CS_GET_WEATHER_INFO : public TS_MESSAGE
{
	uint32_t region_id;

	static const uint16_t packetID = 903;
};




struct TS_SC_WEATHER_INFO : public TS_MESSAGE
{
	uint32_t region_id;
	uint16_t weather_id;

	static const uint16_t packetID = 902;
};




struct TS_SC_EMOTION : public TS_MESSAGE
{
	uint32_t handle;
	int32_t emotion;

	static const uint16_t packetID = 1201;
};




struct TS_CS_EMOTION : public TS_MESSAGE
{
	int32_t emotion;

	static const uint16_t packetID = 1202;
};



struct TS_CS_GAME_TIME : public TS_MESSAGE
{

	static const uint16_t packetID = 1100;
};



struct TS_SC_GAME_TIME : public TS_MESSAGE
{
	uint32_t t;
	int64_t game_time;

	static const uint16_t packetID = 1101;
};




struct TS_CS_AUCTION_SEARCH : public TS_MESSAGE
{
	int32_t category_id;
	int32_t sub_category_id;
	char keyword[31];
	int32_t page_num;
	bool is_equipable;
	static const uint16_t packetID = 1300;
};




struct TS_AUCTION_INFO
{
	int32_t auction_uid;
	TS_ITEM_BASE_INFO item_info;
	int8_t duration_type;
	int64_t bidded_price;
	int64_t instant_purchase_price;
};




struct TS_SC_AUCTION_SEARCH : public TS_MESSAGE
{
	struct SEARCHED_AUCTION_INFO : public TS_AUCTION_INFO
	{
		char seller_name[31];
		int8_t flag;
	};
	int32_t page_num;
	int32_t total_page_count;
	int32_t auction_info_count;
	TS_SC_AUCTION_SEARCH::SEARCHED_AUCTION_INFO auction_info[40];

	static const uint16_t packetID = 1301;
};




struct TS_CS_AUCTION_SELLING_LIST : public TS_MESSAGE
{
	int32_t page_num;

	static const uint16_t packetID = 1302;
};






struct TS_SC_AUCTION_SELLING_LIST : public TS_MESSAGE
{
	struct REGISTERED_AUCTION_INFO : public TS_AUCTION_INFO
	{
		int8_t status;
	};

	int32_t page_num;
	int32_t total_page_count;
	int32_t auction_info_count;
	TS_SC_AUCTION_SELLING_LIST::REGISTERED_AUCTION_INFO auction_info[40];

	static const uint16_t packetID = 1303;
};




struct TS_CS_AUCTION_BIDDED_LIST : public TS_MESSAGE
{
	int32_t page_num;

	static const uint16_t packetID = 1304;
};




struct TS_SC_AUCTION_BIDDED_LIST : public TS_MESSAGE
{
	struct BIDDED_AUCTION_LIST : public TS_AUCTION_INFO
	{
		int8_t status;
	};

	int32_t page_num;
	int32_t total_page_count;
	int32_t auction_info_count;
	TS_SC_AUCTION_BIDDED_LIST::BIDDED_AUCTION_LIST auction_info[40];

	static const uint16_t packetID = 1305;
};




struct TS_CS_AUCTION_BID : public TS_MESSAGE
{
	int32_t auction_uid;
	int64_t bidding_price;

	static const uint16_t packetID = 1306;
};




struct TS_CS_AUCTION_INSTANT_PURCHASE : public TS_MESSAGE
{
	int32_t auction_uid;

	static const uint16_t packetID = 1308;
};




struct TS_CS_AUCTION_REGISTER : public TS_MESSAGE
{
	uint32_t item_handle;
	int32_t item_count;
	int64_t start_price;
	int64_t instant_purchase_price;
	int8_t duration_type;

	static const uint16_t packetID = 1309;
};




struct TS_CS_AUCTION_CANCEL : public TS_MESSAGE
{
	int32_t auction_uid;

	static const uint16_t packetID = 1310;
};




struct TS_CS_ITEM_KEEPING_LIST : public TS_MESSAGE
{
	int32_t page_num;

	static const uint16_t packetID = 1350;
};




struct TS_SC_ITEM_KEEPING_LIST : public TS_MESSAGE
{
	struct TS_ITEM_KEEPING_INFO
	{
		int32_t keeping_uid;
		TS_ITEM_BASE_INFO item_info;
		int32_t duration;
		int8_t keeping_type;
		int32_t related_item_code;
		int32_t related_item_enhance;
		int32_t related_item_level;
	};

	int32_t page_num;
	int32_t total_page_count;
	int32_t keeping_info_count;
	TS_SC_ITEM_KEEPING_LIST::TS_ITEM_KEEPING_INFO keeping_info[40];

	static const uint16_t packetID = 1351;
};




struct TS_CS_ITEM_KEEPING_TAKE : public TS_MESSAGE
{
	int32_t keeping_uid;

	static const uint16_t packetID = 1352;
};




struct TS_CS_HUNTAHOLIC_INSTANCE_LIST : public TS_MESSAGE
{
	int32_t page;

	static const uint16_t packetID = 4000;
};



struct TS_HUNTAHOLIC_INSTANCE_INFO
{
	int32_t instance_no;
	char name[31];
	int8_t current_member_count;
	int8_t max_member_count;
	bool require_password;
};


struct TS_SC_HUNTAHOLIC_INSTANCE_LIST : public TS_MESSAGE_WNA
{
	int32_t huntaholic_id;
	int32_t page;
	int32_t count;
	int32_t total_page;

	TS_HUNTAHOLIC_INSTANCE_INFO instances[0]; //Not sure

	static const uint16_t packetID = 4001;
};




struct TS_SC_HUNTAHOLIC_INSTANCE_INFO : public TS_MESSAGE
{
	TS_HUNTAHOLIC_INSTANCE_INFO info;

	static const uint16_t packetID = 4002;
};


struct TS_CS_HUNTAHOLIC_CREATE_INSTANCE : public TS_MESSAGE
{
	char name[31];
	int8_t max_member_count;
	char password[17];

	static const uint16_t packetID = 4003;
};



struct TS_CS_HUNTAHOLIC_JOIN_INSTANCE : public TS_MESSAGE
{
	int32_t instance_no;
	char password[17];

	static const uint16_t packetID = 4004;
};



struct TS_CS_HUNTAHOLIC_LEAVE_INSTANCE : public TS_MESSAGE
{

	static const uint16_t packetID = 4005;
};



struct TS_SC_HUNTAHOLIC_HUNTING_SCORE : public TS_MESSAGE
{
	int32_t huntaholic_id;
	int32_t personal_kill_count;
	int32_t personal_score;
	int32_t kill_count;
	int32_t score;
	/* long */ double point_advantage;
	/* long */ double point_rate;
	int32_t gain_point;
	int8_t result_type;

	static const uint16_t packetID = 4006;
};




struct TS_SC_HUNTAHOLIC_UPDATE_SCORE : public TS_MESSAGE
{
	int32_t kill_count;
	int32_t score;

	static const uint16_t packetID = 4007;
};



struct TS_CS_HUNTAHOLIC_LEAVE_LOBBY : public TS_MESSAGE
{

	static const uint16_t packetID = 4008;
};



struct TS_SC_HUNTAHOLIC_BEGIN_HUNTING : public TS_MESSAGE
{
	uint32_t begin_time;

	static const uint16_t packetID = 4009;
};



struct TS_SC_HUNTAHOLIC_MAX_POINT_REACHED : public TS_MESSAGE
{

	static const uint16_t packetID = 4010;
};
typedef struct TS_SC_HUNTAHOLIC_MAX_POINT_REACHED TS_SC_HUNTAHOLIC_MAX_POINT_ACHIEVED; 	//fixme: not sure


struct TS_CS_HUNTAHOLIC_BEGIN_HUNTING : public TS_MESSAGE
{
};


struct TS_SC_HUNTAHOLIC_BEGIN_COUNTDOWN : public TS_MESSAGE
{
};



struct TS_CS_INSTANCE_GAME_ENTER : public TS_MESSAGE
{
	int32_t instance_game_type;
};



struct TS_CS_INSTANCE_GAME_EXIT : public TS_MESSAGE
{
};


struct TS_CS_INSTANCE_GAME_SCORE_REQUEST : public TS_MESSAGE
{
};



struct TS_SC_INSTANCE_GAME_SCORE_REQUEST : public TS_MESSAGE
{
	uint32_t holicpoint;
	uint32_t bearroad_ranking;
	uint32_t deathmatch_kill_count;
	uint32_t deathmatch_death_count;
};



struct TS_CS_COMPETE_REQUEST : public TS_MESSAGE
{
	int8_t compete_type;
	char requestee[31];

	static const uint16_t packetID = 4500;
};


struct TS_SC_COMPETE_REQUEST : public TS_MESSAGE
{
	int8_t compete_type;
	char requester[31];

	static const uint16_t packetID = 4501;
};


struct TS_CS_COMPETE_ANSWER : public TS_MESSAGE
{
	int8_t compete_type;
	int8_t answer_type;

	static const uint16_t packetID = 4502;
};


struct TS_SC_COMPETE_ANSWER : public TS_MESSAGE
{
	int8_t compete_type;
	int8_t answer_type;
	char requestee[31];

	static const uint16_t packetID = 4503;
};



struct TS_SC_COMPETE_COUNTDOWN : public TS_MESSAGE
{
	int8_t compete_type;
	char competitor[31];
	uint32_t handle_competitor;

	static const uint16_t packetID = 4504;
};



struct TS_SC_COMPETE_START : public TS_MESSAGE
{
	int8_t compete_type;
	char competitor[31];

	static const uint16_t packetID = 4505;
};


struct TS_SC_COMPETE_END : public TS_MESSAGE
{
	int8_t compete_type;
	int8_t end_type;
	char winner[31];
	char loser[31];

	static const uint16_t packetID = 4506;
};


struct TS_CS_RANKING_TOP_RECORD : public TS_MESSAGE
{
	int8_t ranking_type;

	static const uint16_t packetID = 5000;
};



struct TS_SC_RANKING_TOP_RECORD : public TS_MESSAGE_WNA
{
	int8_t ranking_type;
	uint16_t requester_rank;
	int64_t requester_score;
	uint16_t record_count;

	struct RANKING_RECORD
	{
		uint16_t rank;
		char ranker_name[31];
		int64_t score;
	} records[0];

	static const uint16_t packetID = 5001;
};



struct TS_CS_REQUEST_FARM_INFO : public TS_MESSAGE
{
};


struct TS_SC_FARM_INFO : public TS_MESSAGE_WNA
{
	int8_t creature_count;


	struct SUMMON_INFO
	{
		int32_t index;
		int64_t exp;
		char name[19];
		int32_t duration;
		int32_t elasped_time;
		int32_t refresh_time;
		int8_t using_cash;
		int8_t using_cracker;
		TS_ITEM_BASE_INFO card_info;
	} creatures[0];

};



struct TS_CS_FOSTER_CREATURE : public TS_MESSAGE_WNA
{
	uint32_t creature_card_handle;
	int32_t ticket_info_count;
	int32_t cracker_info_count;

	struct TICKET_INFO
	{
		uint32_t ticket_handle;
		int32_t ticket_count;
	} tickets[0];

	struct CRACKER_INFO
	{
		uint32_t cracker_handle;
		int32_t cracker_count;
	} crakers[0];
};



struct TS_SC_RESULT_FOSTER : public TS_MESSAGE
{
	int8_t result;
};



struct TS_CS_RETRIEVE_CREATURE : public TS_MESSAGE
{
	uint32_t creature_card_handle;
};



struct TS_SC_RESULT_RETRIEVE : public TS_MESSAGE
{
	int8_t result;
};



struct TS_CS_NURSE_CREATURE : public TS_MESSAGE
{
	uint32_t creature_card_handle;
};



struct TS_SC_RESULT_NURSE : public TS_MESSAGE
{
	int8_t result;
};


struct TS_CS_REQUEST_FARM_MARKET : public TS_MESSAGE
{
};


struct TS_CS_OPEN_ITEM_SHOP : public TS_MESSAGE
{

	static const uint16_t packetID = 10000;
};



struct TS_SC_OPEN_ITEM_SHOP : public TS_MESSAGE
{
	int32_t client_id;
	int32_t account_id;
	int32_t one_time_password;
	char raw_server_name[32];

	static const uint16_t packetID = 10001;
};




struct TS_SC_REQUEST_SECURITY_NO : public TS_MESSAGE
{
	int32_t mode;

	static const uint16_t packetID = 9004;
};




struct TS_CS_SECURITY_NO : public TS_MESSAGE
{
	int32_t mode;
	char security_no[19];

	static const uint16_t packetID = 9005;
};



struct TS_SU_LOGIN : public TS_MESSAGE
{
	char server_name[21];
};



struct TS_US_LOGIN_RESULT : public TS_MESSAGE
{
	uint16_t result;
};




struct TS_SU_REQUEST_UPLOAD : public TS_MESSAGE
{
	int32_t client_id;
	int32_t account_id;
	int32_t guild_sid;
	int32_t one_time_password;
	int8_t type;
};




struct TS_US_REQUEST_UPLOAD : public TS_MESSAGE
{
	uint16_t result;
};




struct TS_US_UPLOAD : public TS_MESSAGE
{
	int32_t guild_id;
	int32_t file_size;
	int8_t filename_length;
	int8_t type;
};

#pragma pack(pop)

#endif // PACKETS_PACKETSTRUCTS_H
