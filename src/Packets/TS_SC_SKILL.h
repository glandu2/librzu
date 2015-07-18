#ifndef PACKETS_TS_SC_SKILL_H
#define PACKETS_TS_SC_SKILL_H

#include "Packets/PacketBaseMessage.h"
#include "Packets/PacketEnums.h"

#pragma pack(push, 1)

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

	static const int packetID = 406;
};

struct TS_SC_SKILL : public TS_MESSAGE
{
	struct CancelType
	{
		uint8_t dummy[1];
	};

	struct CompleteType
	{
		uint8_t dummy[1];
	};
	struct CastType
	{
		uint32_t tm;
		uint16_t nErrorCode;
	};

	struct FireType //type 0
	{
		bool bMultiple;
		float range;
		int8_t target_count;
		int8_t fire_count;
		uint16_t count;

		enum DamageType : uint8_t
		{
			TYPE_NONE = 0x0,
			TYPE_FIRE = 0x1,
			TYPE_WATER = 0x2,
			TYPE_WIND = 0x3,
			TYPE_EARTH = 0x4,
			TYPE_LIGHT = 0x5,
			TYPE_DARK = 0x6,
			TYPE_COUNT = 0x7,
		};

		struct HitDetails {
			enum Type : uint8_t
			{
				DAMAGE = 0,
				MAGIC_DAMAGE = 1,
				DAMAGE_WITH_KNOCK_BACK = 2,
				RESULT = 10,
				ADD_HP = 20,
				ADD_MP = 21,
				ADD_HP_MP_SP = 22,
				REBIRTH = 23,
				RUSH = 30,
				CHAIN_DAMAGE = 40,
				CHAIN_MAGIC_DAMAGE = 41,
				CHAIN_HEAL = 42,
				NOT_USE = 100,
			} type;

			struct Damage  //type 1
			{
				uint32_t hTarget;
				int32_t target_hp;
				DamageType damage_type;
				int32_t damage;
				int32_t flag;
				uint16_t elemental_damage[7];
			};

			struct DamageWithKnockBack
			{
				uint32_t hTarget;
				int32_t target_hp;
				DamageType damage_type;
				int32_t damage;
				int32_t flag;
				uint16_t elemental_damage[7];
				float x;
				float y;
				int16_t speed;
				uint32_t knock_back_time;
			};

			struct Result
			{
				uint32_t hTarget;
				bool bResult;
				int32_t success_type;
			};

			struct AddHP  //type 0x14
			{
				uint32_t hTarget;
				int32_t target_hp;
				int32_t nIncHP;
			};

			struct AddHPMPSP
			{
				uint32_t hTarget;
				int32_t target_hp;
				int32_t nIncHP;
				int32_t nIncMP;
				int32_t nIncSP;
				int16_t target_mp;
			};

			struct Rush
			{
				uint32_t hTarget;
				bool bResult;
				float x;
				float y;
				float face;
				int8_t speed;
			};

			struct Rebirth
			{
				uint32_t hTarget;
				int32_t target_hp;
				int32_t nIncHP;
				int32_t nIncMP;
				int32_t nRecoveryEXP;
				int16_t target_mp;
			};

			struct ChainDamage
			{
				uint32_t hTarget;
				int32_t target_hp;
				int8_t damage_type;
				int32_t damage;
				int32_t flag;
				uint16_t elemental_damage[7];
				uint32_t hFrom;
			};

			struct ChainHeal
			{
				uint32_t hTarget;
				int32_t target_hp;
				int32_t nIncHP;
				uint32_t hFrom;
			};

			union {
				Damage damage;
				DamageWithKnockBack damageWithKnockBack;
				Result result;
				AddHP addHP;
				AddHPMPSP addHPMPSP;
				Rush rush;
				Rebirth rebirth;
				ChainDamage chainDamage;
				ChainHeal chainHeal;
			};
		} hits[];
	};

	uint16_t skill_id;
	uint8_t skill_level;
	uint32_t caster;
	uint32_t target;
	float x;
	float y;
	float z;
	uint8_t layer;
	enum Type : uint8_t
	{
		FIRE = 0,
		CASTING = 1,
		CASTING_UPDATE = 2,
		CANCEL = 3,
		REGION_FIRE = 4,
		COMPLETE = 5,
	} type;
	int32_t hp_cost;
	int32_t mp_cost;
	int32_t caster_hp;
	int32_t caster_mp;

	union {
		CancelType cancel;
		CompleteType complete;
		CastType cast;
		FireType fire;
	};

	static const int packetID = 401;
};
#pragma pack(pop)

#endif // PACKETS_TS_SC_ENTER_H
