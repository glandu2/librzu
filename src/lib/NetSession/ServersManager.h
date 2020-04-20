#ifndef SERVERSMANAGER_H
#define SERVERSMANAGER_H

#include "Core/Object.h"
#include <string>
#include <unordered_map>
#include <vector>

class StartableObject;
class IWritableConsole;
template<typename T> class cval;

class RZU_EXTERN ServersManager : public Object {
	DECLARE_CLASSNAME(ServersManager, 0)
public:
	ServersManager();
	~ServersManager();

	bool start();
	bool stop();
	bool forceStop();  ///< Stop all servers even those with stopAllKeepRunning set

	bool start(const std::string& name);
	bool stop(const std::string& name);

	StartableObject* getServer(const std::string& name);
	void addServer(const char* name, StartableObject* server, cval<bool>* autoStart, bool stopAllKeepRunning = false);

	static ServersManager* getInstance() { return instance; }

protected:
	static void commandStartServer(IWritableConsole* console, const std::vector<std::string>& args);
	static void commandStopServer(IWritableConsole* console, const std::vector<std::string>& args);
	static void commandListServers(IWritableConsole* console, const std::vector<std::string>& args);

private:
	struct ServerInfo {
		ServerInfo(StartableObject* server, cval<bool>* autoStart, bool stopAllKeepRunning)
		    : server(server), autoStart(autoStart), stopAllKeepRunning(stopAllKeepRunning) {}

		StartableObject* server;
		cval<bool>* autoStart;
		bool stopAllKeepRunning;
	};
	std::unordered_map<std::string, ServerInfo*> servers;

	static ServersManager* instance;
};

#endif  // SERVERSMANAGER_H
