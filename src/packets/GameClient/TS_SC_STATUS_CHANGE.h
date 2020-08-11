#pragma once

#include "Packet/PacketDeclaration.h"


enum TS_CREATURE_STATUS : uint32_t {
  // Creature
  TCS_FlagBattleMode = 1 << 0,
  TCS_FlagInvisible   = 1 << 1,

  // Monster
  TCS_FlagDead = 1 << 8,

  // Npc
  TCS_FlagHasStartableQuest	= 1 << 8,
  TCS_FlagHasInProgressQuest	= 1 << 9,
  TCS_FlagHasFinishableQuest	= 1 << 10,

  // Player
  TCS_FlagSitdown = 1 << 8,
  TCS_FlagBuyBooth = 1 << 9,
  TCS_FlagSellBooth = 1 << 10,
  TCS_FlagPkOn = 1 << 11,
  TCS_FlagBloody = 1 << 12,
  TCS_FlagDemoniac = 1 << 13,
  TCS_FlagGm = 1 << 14,
  TCS_FlagDungeonOriginalOwner = 1 << 15,
  TCS_FlagWalking = 1 << 16,
  TCS_FlagDungeonOriginalSieger = 1 << 17,
  TCS_FlagShovelingSearch = 1 << 18,
  TCS_FlagShovelingApproach = 1 << 19,
  TCS_FlagShovelingDig = 1 << 20,
  TCS_FlagCompeting = 1 << 21,
  TCS_FlagBattleArenaTeam0 = 1 << 22,
  TCS_FlagBattleArenaTeam1 = 1 << 23,
};

#define TS_SC_STATUS_CHANGE_DEF(_) \
	_(simple) (ar_handle_t, handle) \
	_(simple) (TS_CREATURE_STATUS, status)

#define TS_SC_STATUS_CHANGE_ID(X) \
	X(500, version < EPIC_9_6_3) \
	X(1500, version >= EPIC_9_6_3)

CREATE_PACKET_VER_ID(TS_SC_STATUS_CHANGE, SessionType::GameClient, SessionPacketOrigin::Server);
#undef TS_SC_STATUS_CHANGE_DEF

