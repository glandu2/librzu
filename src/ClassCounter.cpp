#include "ClassCounter.h"

//For detail about this variable, see ClassCounter.h
unsigned int ClassCounter__internal_object_engine_class_counter = 0;

std::map<std::string, unsigned long*>& getObjectsCount() {
	static std::map<std::string, unsigned long*> objectsCount;

	return objectsCount;
}

//generated with notepad++ with this pattern: (.*) ->
//#include "\1.h"\nDECLARE_CLASSCOUNT_STATIC\(\1\)\n

#include "Object.h"
DECLARE_CLASSCOUNT_STATIC(Object)

#include "BanManager.h"
DECLARE_CLASSCOUNT_STATIC(BanManager)

#include "ConfigInfo.h"
DECLARE_CLASSCOUNT_STATIC(ConfigInfo)

#include "ConfigValue.h"
DECLARE_CLASSCOUNT_STATIC(ConfigValue)

#include "DbConnection.h"
DECLARE_CLASSCOUNT_STATIC(DbConnection)

#include "DbQueryJob.h"
DECLARE_CLASSCOUNT_STATIC(DbQueryBinding)

#include "DelegatedPacketSession.h"
DECLARE_CLASSCOUNT_STATIC(DelegatedPacketSession)

#include "EventLoop.h"
DECLARE_CLASSCOUNT_STATIC(EventLoop)

#include "Log.h"
DECLARE_CLASSCOUNT_STATIC(Log)

#include "Pipe.h"
DECLARE_CLASSCOUNT_STATIC(Pipe)

#include "RappelzServer.h"
DECLARE_CLASSCOUNT_STATIC(RappelzServerCommon)

#include "PacketSession.h"
DECLARE_CLASSCOUNT_STATIC(PacketSession)

#include "RC4Cipher.h"
DECLARE_CLASSCOUNT_STATIC(RC4Cipher)

#include "Socket.h"
DECLARE_CLASSCOUNT_STATIC(Socket)

#include "SocketSession.h"
DECLARE_CLASSCOUNT_STATIC(SocketSession)
