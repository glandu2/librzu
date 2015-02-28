#include "ServersManager.h"
#include "ConfigInfo.h"
#include "BanManager.h"

ServersManager* ServersManager::instance = nullptr;

ServersManager::ServersManager()
{
	if(instance == nullptr)
		instance = this;
	else
		error("Several ServersManager instance !\n");
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

void ServersManager::addServer(const char* name, StartableObject *server, cval<bool>& autoStart) {
	ServerInfo* serverInfo = new ServerInfo(server, &autoStart);

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

		if(serverInfo->autoStart->getBool()) {
			serverInfo->server->start();
		}
	}

	return servers.size() > 0;
}

bool ServersManager::stop() {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it, itEnd;

	for(it = servers.cbegin(), itEnd = servers.cend(); it != itEnd; ++it) {
		ServerInfo* serverInfo = it->second;

		serverInfo->server->stop();
	}

	return servers.size() > 0;
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
