#ifndef PACKETSTRUCTSNAME_H
#define PACKETSTRUCTSNAME_H

#include <stddef.h>

enum class SessionType { Any, AuthClient, AuthGame, GameClient, UploadClient, UploadGame };
enum class SessionPacketDir { ToServer, ToClient };

const char* getPacketName(unsigned int id, SessionType sessionType, SessionPacketDir packetDir);

#endif
