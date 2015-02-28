#ifndef SERVERSMANAGER_H
#define SERVERSMANAGER_H

#include "Object.h"
#include <unordered_map>
#include "StartableObject.h"
#include <string>
#include "ConfigParamVal.h"

class BanManager;

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

	StartableObject *getServer(const std::string& name);
	void addServer(const char* name, StartableObject* server, cval<bool> &autoStart);

	static ServersManager* getInstance() { return instance; }

private:
	struct ServerInfo {
		ServerInfo(StartableObject* server, cval<bool>* autoStart) :
			server(server), autoStart(autoStart) {}

		StartableObject* server;
		cval<bool>* autoStart;
	};
	std::unordered_map<std::string, ServerInfo*> servers;

	static ServersManager* instance;
};

#endif // SERVERSMANAGER_H
