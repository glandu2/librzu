#ifndef PACKETS_TS_SC_MOUNT_SUMMON_H
#define PACKETS_TS_SC_MOUNT_SUMMON_H

#include "Packet/PacketDeclaration.h"

#define TS_SC_MOUNT_SUMMON_DEF(_) \
	_(simple)(ar_handle_t, handle) \
	_(simple)(ar_handle_t, summon_handle) \
	_(simple)(float, x) \
	_(simple)(float, y) \
	_(simple)(bool, success)

CREATE_PACKET(TS_SC_MOUNT_SUMMON, 320);
#undef TS_SC_MOUNT_SUMMON_DEF

#endif // PACKETS_TS_SC_MOUNT_SUMMON_H
