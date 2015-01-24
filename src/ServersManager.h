#ifndef SERVERSMANAGER_H
#define SERVERSMANAGER_H

#include "Object.h"
#include <unordered_map>
#include "SessionServer.h"
#include <string>

class BanManager;

class ConfigValue;

class LIB_EXTERN ServersManager : public Object
{
	DECLARE_CLASSNAME(ServersManager, 0)
public:
	ServersManager();
	~ServersManager();

	bool stop();
	bool start();

	bool start(const std::string& name);
	bool stop(const std::string& name);

	SessionServerCommon* getServer(const std::string& name);
	void addServer(const char* name, SessionServerCommon* server, ConfigValue& listenIp, ConfigValue& listenPort, ConfigValue& autoStart, BanManager* banManager = nullptr);

	static ServersManager* getInstance() { return instance; }

private:
	struct ServerInfo {
		ServerInfo(SessionServerCommon* server, ConfigValue* listenIp, ConfigValue* listenPort, ConfigValue* autoStart, BanManager* banManager = nullptr) :
			server(server), listenIp(listenIp), listenPort(listenPort), autoStart(autoStart), banManager(banManager) {}

		SessionServerCommon* server;
		ConfigValue* listenIp;
		ConfigValue* listenPort;
		ConfigValue* autoStart;
		BanManager* banManager;
	};
	std::unordered_map<std::string, ServerInfo*> servers;

	static ServersManager* instance;
};

#endif // SERVERSMANAGER_H
