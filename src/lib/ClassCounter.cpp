#include "ClassCounter.h"

// For detail about this variable, see ClassCounter.h
unsigned int ClassCounter__internal_object_engine_class_counter = 0;

std::map<std::string, unsigned long*>& getObjectsCount() {
	static std::map<std::string, unsigned long*> objectsCount;

	return objectsCount;
}

	// generated with notepad++ with this pattern: (.*) ->
	// #include "\1.h"\nDECLARE_CLASSCOUNT_STATIC\(\1\)\n

#include "Config/ConfigInfo.h"
DECLARE_CLASSCOUNT_STATIC(ConfigInfo)

#include "Config/ConfigValue.h"
DECLARE_CLASSCOUNT_STATIC(ConfigValue)

#include "Console/ConsoleSession.h"
DECLARE_CLASSCOUNT_STATIC(ConsoleSession)

#include "Core/Object.h"
DECLARE_CLASSCOUNT_STATIC(Object)

#include "Core/EventLoop.h"
DECLARE_CLASSCOUNT_STATIC(EventLoop)

#include "Core/Log.h"
DECLARE_CLASSCOUNT_STATIC(Log)

#include "Database/DbConnection.h"
DECLARE_CLASSCOUNT_STATIC(DbConnection)

#include "Database/DbQueryJob.h"
DECLARE_CLASSCOUNT_STATIC(DbQueryBinding)

#include "NetSession/BanManager.h"
DECLARE_CLASSCOUNT_STATIC(BanManager)

#include "NetSession/ClientAuthSession.h"
DECLARE_CLASSCOUNT_STATIC(ClientAuthSession)

#include "NetSession/DelegatedPacketSession.h"
DECLARE_CLASSCOUNT_STATIC(DelegatedPacketSession)

#include "NetSession/SessionServer.h"
DECLARE_CLASSCOUNT_STATIC(SessionServerCommon)

#include "NetSession/PacketSession.h"
DECLARE_CLASSCOUNT_STATIC(PacketSession)

#include "NetSession/SocketSession.h"
DECLARE_CLASSCOUNT_STATIC(SocketSession)

#include "NetSession/TelnetSession.h"
DECLARE_CLASSCOUNT_STATIC(TelnetSession)

#include "Stream/Pipe.h"
DECLARE_CLASSCOUNT_STATIC(Pipe)

#include "Stream/Socket.h"
DECLARE_CLASSCOUNT_STATIC(Socket)
