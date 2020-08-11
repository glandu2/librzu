#pragma once

#include "../Extern.h"
#include "PacketEpics.h"
#include <stdint.h>

enum class SessionType { Any, AuthClient, AuthGame, GameClient, UploadClient, UploadGame, NUMBER };
enum class SessionPacketOrigin { Any, Server, Client, NUMBER };
typedef int32_t packet_type_id_t;

namespace PacketMetadata {
RZU_EXTERN SessionPacketOrigin getPacketOriginFromDirection(bool outgoing, SessionPacketOrigin selfSessionPacketOrigin);

RZU_EXTERN const char* getPacketName(unsigned int id,
                                     SessionType sessionType,
                                     SessionPacketOrigin packetDir,
                                     packet_version_t version);

RZU_EXTERN packet_type_id_t convertPacketIdToTypeId(uint16_t id,
                                                    SessionType sessionType,
                                                    SessionPacketOrigin packetDir,
                                                    packet_version_t version);
}  // namespace PacketMetadata
