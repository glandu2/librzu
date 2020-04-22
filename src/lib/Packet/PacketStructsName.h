#pragma once

#include "../Extern.h"

enum class SessionType { Any, AuthClient, AuthGame, GameClient, UploadClient, UploadGame };
enum class SessionPacketOrigin { Any, Server, Client };

SessionPacketOrigin getPacketOriginFromDirection(bool outgoing, SessionPacketOrigin selfSessionPacketOrigin);

RZU_EXTERN const char* getPacketName(unsigned int id, SessionType sessionType, SessionPacketOrigin packetDir);

