#include "ServersManager.h"
#include "Config/ConfigParamVal.h"
#include "BanManager.h"
#include "StartableObject.h"
#include "Console/ConsoleCommands.h"

ServersManager* ServersManager::instance = nullptr;

ServersManager::ServersManager()
{
	if(instance == nullptr) {
		instance = this;
		ConsoleCommands::get()->addCommand("server.list", 0, &commandListServers,
										   "List all available servers",
										   "server.list : list all servers\r\n");
		ConsoleCommands::get()->addCommand("server.start", "start", 1, &commandStartServer,
										   "Start a server (use server.list to get server names)",
										   "server.start <server name> : Start <server name> server\r\n"
										   "server.start all           : Start all servers\r\n");
		ConsoleCommands::get()->addCommand("server.stop", "stop", 1, &commandStopServer,
										   "Stop a server (use server.list to get server names)",
										   "server.stop <server name> : Stop <server name> server\r\n"
										   "server.stop all           : Stop all servers except console server\r\n");
	} else {
		log(LL_Error, "Several ServersManager instance !\n");
	}
}

ServersManager::~ServersManager()
{
	if(instance == this)
		instance = nullptr;

	std::unordered_map<std::string, ServerInfo*>::iterator it, itEnd;

	for(it = servers.begin(), itEnd = servers.end(); it != itEnd; ++it) {
		ServerInfo* & server = it->second;
		delete server;
		server = nullptr;
	}
}

void ServersManager::addServer(const char* name, StartableObject *server, cval<bool>* autoStart, bool stopAllKeepRunning) {
	ServerInfo* serverInfo = new ServerInfo(server, autoStart, stopAllKeepRunning);

	server->setName(name);

	servers.insert(std::pair<std::string, ServerInfo*>(std::string(name), serverInfo));
}

StartableObject* ServersManager::getServer(const std::string& name) {
	std::unordered_map<std::string, ServerInfo*>::iterator it = servers.find(name);
	if(it == servers.end())
		return nullptr;

	return it->second->server;
}

bool ServersManager::start() {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it, itEnd;

	for(it = servers.cbegin(), itEnd = servers.cend(); it != itEnd; ++it) {
		ServerInfo* serverInfo = it->second;

		if(!serverInfo->autoStart || serverInfo->autoStart->getBool()) {
			serverInfo->server->start();
		}
	}

	return !servers.empty();
}

bool ServersManager::stop() {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it, itEnd;

	for(it = servers.cbegin(), itEnd = servers.cend(); it != itEnd; ++it) {
		ServerInfo* serverInfo = it->second;
		if(serverInfo->stopAllKeepRunning == false)
			serverInfo->server->stop();
	}

	return !servers.empty();
}

bool ServersManager::forceStop() {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it, itEnd;

	for(it = servers.cbegin(), itEnd = servers.cend(); it != itEnd; ++it) {
		ServerInfo* serverInfo = it->second;
		serverInfo->server->stop();
	}

	return !servers.empty();
}

bool ServersManager::start(const std::string& name) {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it = servers.find(name);
	if(it != servers.end()) {
		ServerInfo* serverInfo = it->second;

		serverInfo->server->start();

		return true;
	}

	return false;
}

bool ServersManager::stop(const std::string& name) {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it = servers.find(name);
	if(it != servers.end()) {
		ServerInfo* serverInfo = it->second;

		serverInfo->server->stop();

		return true;
	}

	return false;
}

void ServersManager::commandListServers(IWritableConsole* console, const std::vector<std::string>& args) {
	ServersManager* instance = ServersManager::getInstance();

	auto it = instance->servers.begin();
	auto itEnd = instance->servers.end();
	for(; it != itEnd; ++it) {
		const std::string& name = it->first;
		console->writef("%s\r\n", name.c_str());
	}
}

void ServersManager::commandStartServer(IWritableConsole* console, const std::vector<std::string>& args) {
	bool ok;
	const std::string& name = args[0];

	if(name == "all")
		ok = ServersManager::getInstance()->start();
	else
		ok = ServersManager::getInstance()->start(name);

	if(!ok) {
		console->writef("Unknown server name : %s\r\n", name.c_str());
	} else {
		console->log("Server %s started via console session\n", name.c_str());
		console->writef("Server %s started\r\n", name.c_str());
	}
}

void ServersManager::commandStopServer(IWritableConsole* console, const std::vector<std::string>& args) {
	bool ok;
	const std::string& name = args[0];

	if(name == "all")
		ok = ServersManager::getInstance()->stop();
	else
		ok = ServersManager::getInstance()->stop(name);

	if(!ok) {
		console->writef("Unknown server name : %s\r\n", name.c_str());
	} else {
		console->log("Server %s stopped via console session\n", name.c_str());
		console->writef("Server %s stopped\r\n", name.c_str());
	}
}
