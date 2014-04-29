#ifndef SERVERSMANAGER_H
#define SERVERSMANAGER_H

#include "Object.h"
#include <unordered_map>
#include "RappelzServer.h"
#include <string>

class BanManager;

class ConfigValue;

class RAPPELZLIB_EXTERN ServersManager : public Object
{
	DECLARE_CLASSNAME(ServersManager, 0)
public:
	ServersManager();
	~ServersManager();

	bool stop();
	bool start();

	bool start(const std::string& name);
	bool stop(const std::string& name);

	RappelzServerCommon* getServer(const std::string& name);
	void addServer(const char* name, RappelzServerCommon* server, ConfigValue& listenIp, ConfigValue& listenPort, ConfigValue& autoStart, BanManager* banManager = nullptr);

	static ServersManager* getInstance() { return instance; }

private:
	struct ServerInfo {
		ServerInfo(RappelzServerCommon* server, ConfigValue* listenIp, ConfigValue* listenPort, ConfigValue* autoStart, BanManager* banManager = nullptr) :
			server(server), listenIp(listenIp), listenPort(listenPort), autoStart(autoStart), banManager(banManager) {}

		RappelzServerCommon* server;
		ConfigValue* listenIp;
		ConfigValue* listenPort;
		ConfigValue* autoStart;
		BanManager* banManager;
	};
	std::unordered_map<std::string, ServerInfo*> servers;

	static ServersManager* instance;
};

#endif // SERVERSMANAGER_H
