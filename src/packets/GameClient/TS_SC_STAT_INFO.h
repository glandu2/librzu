#ifndef PACKETS_TS_SC_STAT_INFO_H
#define PACKETS_TS_SC_STAT_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_STAT_INFO_BASE_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
  simple_(int16_t, stat_id) \
  simple_(int16_t, strength) \
  simple_(int16_t, vital) \
  simple_(int16_t, dexterity) \
  simple_(int16_t, agility) \
  simple_(int16_t, intelligence) \
  simple_(int16_t, mentality) \
  simple_(int16_t, luck)
CREATE_STRUCT(TS_STAT_INFO_BASE);

#define TS_STAT_INFO_ATTRIB_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
  simple_(int16_t, nCritical) \
  simple_(int16_t, nCriticalPower) \
  simple_(int16_t, nAttackPointRight) \
  simple_(int16_t, nAttackPointLeft) \
  simple_(int16_t, nDefence) \
  simple_(int16_t, nBlockDefence) \
  simple_(int16_t, nMagicPoint) \
  simple_(int16_t, nMagicDefence) \
  simple_(int16_t, nAccuracyRight) \
  simple_(int16_t, nAccuracyLeft) \
  simple_(int16_t, nMagicAccuracy) \
  simple_(int16_t, nAvoid) \
  simple_(int16_t, nMagicAvoid) \
  simple_(int16_t, nBlockChance) \
  simple_(int16_t, nMoveSpeed) \
  simple_(int16_t, nAttackSpeed) \
  simple_(int16_t, nAttackRange) \
  simple_(int32_t, nMaxWeight) \
  simple_(int16_t, nCastingSpeed) \
  simple_(int16_t, nCoolTimeSpeed) \
  simple_(int16_t, nItemChance) \
  simple_(int16_t, nHPRegenPercentage) \
  simple_(int16_t, nHPRegenPoint) \
  simple_(int16_t, nMPRegenPercentage) \
  simple_(int16_t, nMPRegenPoint) \
  simple_(int16_t, nPerfectBlock) \
  simple_(int16_t, nMagicalDefIgnore) \
  simple_(int16_t, nMagicalDefIgnoreRatio) \
  simple_(int16_t, nPhysicalDefIgnore) \
  simple_(int16_t, nPhysicalDefIgnoreRatio) \
  simple_(int16_t, nMagicalPenetration) \
  simple_(int16_t, nMagicalPenetrationRatio) \
  simple_(int16_t, nPhysicalPenetration) \
  simple_(int16_t, nPhysicalPenetrationRatio)
CREATE_STRUCT(TS_STAT_INFO_ATTRIB);

#define TS_SC_STAT_INFO_DEF(simple_, array_, dynarray_, count_, string_, dynstring_) \
	simple_ (uint32_t, handle) \
	simple_ (TS_STAT_INFO_BASE, stat) \
	simple_ (TS_STAT_INFO_ATTRIB, attribute) \
	simple_ (uint8_t, type)

CREATE_PACKET(TS_SC_STAT_INFO, 1000);

#endif // PACKETS_TS_SC_STAT_INFO_H
