#ifndef SERVERSMANAGER_H
#define SERVERSMANAGER_H

#include "Core/Object.h"
#include <unordered_map>
#include <string>

class BanManager;
class StartableObject;
template<typename T> class cval;

class RZU_EXTERN ServersManager : public Object
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
	void addServer(const char* name, StartableObject* server, cval<bool> &autoStart, bool stopAllKeepRunning = false);

	static ServersManager* getInstance() { return instance; }

private:
	struct ServerInfo {
		ServerInfo(StartableObject* server, cval<bool>* autoStart, bool stopAllKeepRunning) :
			server(server), autoStart(autoStart), stopAllKeepRunning(stopAllKeepRunning) {}

		StartableObject* server;
		cval<bool>* autoStart;
		bool stopAllKeepRunning;
	};
	std::unordered_map<std::string, ServerInfo*> servers;

	static ServersManager* instance;
};

#endif // SERVERSMANAGER_H
