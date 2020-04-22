#pragma once

#include "Packet/PacketDeclaration.h"

#define TS_CS_GET_SUMMON_SETUP_INFO_DEF(_) \
	_(simple)(bool, show_dialog)

CREATE_PACKET(TS_CS_GET_SUMMON_SETUP_INFO, 324);
#undef TS_CS_GET_SUMMON_SETUP_INFO_DEF

