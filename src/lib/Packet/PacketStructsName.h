#ifndef PACKETSTRUCTSNAME_H
#define PACKETSTRUCTSNAME_H

#include "../Extern.h"
#include <stddef.h>

enum class SessionType { Any, AuthClient, AuthGame, GameClient, UploadClient, UploadGame };
enum class SessionPacketOrigin { Server, Client };

RZU_EXTERN const char* getPacketName(unsigned int id, SessionType sessionType, SessionPacketOrigin packetDir);

#endif
