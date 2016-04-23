#ifndef PACKETS_TS_SC_STAT_INFO_H
#define PACKETS_TS_SC_STAT_INFO_H

#include "Packet/PacketDeclaration.h"

#define TS_STAT_INFO_BASE_DEF(_) \
  _(simple)(int16_t, stat_id) \
  _(simple)(int16_t, strength) \
  _(simple)(int16_t, vital) \
  _(simple)(int16_t, dexterity) \
  _(simple)(int16_t, agility) \
  _(simple)(int16_t, intelligence) \
  _(simple)(int16_t, mentality) \
  _(simple)(int16_t, luck)
CREATE_STRUCT(TS_STAT_INFO_BASE);

#define TS_STAT_INFO_ATTRIB_DEF(_) \
  _(simple)(int16_t, nCritical) \
  _(simple)(int16_t, nCriticalPower) \
  _(simple)(int16_t, nAttackPointRight) \
  _(simple)(int16_t, nAttackPointLeft) \
  _(simple)(int16_t, nDefence) \
  _(simple)(int16_t, nBlockDefence) \
  _(simple)(int16_t, nMagicPoint) \
  _(simple)(int16_t, nMagicDefence) \
  _(simple)(int16_t, nAccuracyRight) \
  _(simple)(int16_t, nAccuracyLeft) \
  _(simple)(int16_t, nMagicAccuracy) \
  _(simple)(int16_t, nAvoid) \
  _(simple)(int16_t, nMagicAvoid) \
  _(simple)(int16_t, nBlockChance) \
  _(simple)(int16_t, nMoveSpeed) \
  _(simple)(int16_t, nAttackSpeed) \
  _(simple)(int16_t, nAttackRange) \
  _(simple)(int32_t, nMaxWeight) \
  _(simple)(int16_t, nCastingSpeed) \
  _(simple)(int16_t, nCoolTimeSpeed) \
  _(simple)(int16_t, nItemChance) \
  _(simple)(int16_t, nHPRegenPercentage) \
  _(simple)(int16_t, nHPRegenPoint) \
  _(simple)(int16_t, nMPRegenPercentage) \
  _(simple)(int16_t, nMPRegenPoint) \
  _(simple)(int16_t, nPerfectBlock, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nMagicalDefIgnore, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nMagicalDefIgnoreRatio, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nPhysicalDefIgnore, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nPhysicalDefIgnoreRatio, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nMagicalPenetration, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nMagicalPenetrationRatio, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nPhysicalPenetration, version >= EPIC_7_3, 20) \
  _(simple)(int16_t, nPhysicalPenetrationRatio, version >= EPIC_7_3, 20)
CREATE_STRUCT(TS_STAT_INFO_ATTRIB);

#define TS_SC_STAT_INFO_DEF(_) \
	_(simple) (uint32_t, handle) \
	_(simple) (TS_STAT_INFO_BASE, stat) \
	_(simple) (TS_STAT_INFO_ATTRIB, attribute) \
	_(simple) (uint8_t, type)

CREATE_PACKET(TS_SC_STAT_INFO, 1000);

#endif // PACKETS_TS_SC_STAT_INFO_H
