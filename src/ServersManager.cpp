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

void ServersManager::addServer(const char* name, SessionServerCommon* server, ConfigValue& listenIp, ConfigValue& listenPort, ConfigValue& autoStart, BanManager* banManager) {
	ServerInfo* serverInfo = new ServerInfo(server, &listenIp, &listenPort, &autoStart, banManager);

	servers.insert(std::pair<std::string, ServerInfo*>(std::string(name), serverInfo));
}

SessionServerCommon* ServersManager::getServer(const std::string& name) {
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
			if(serverInfo->banManager)
				serverInfo->banManager->loadFile();

			if(serverInfo->server->isListening() == false) {
				info("Starting server %s on %s:%d\n", it->first.c_str(), serverInfo->listenIp->getString().c_str(), serverInfo->listenPort->getInt());

				serverInfo->server->startServer(serverInfo->listenIp->getString(),
												serverInfo->listenPort->getInt(),
												serverInfo->banManager);
			} else {
				info("Server %s already started\n", it->first.c_str());
			}
		}
	}

	return servers.size() > 0;
}

bool ServersManager::stop() {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it, itEnd;

	for(it = servers.cbegin(), itEnd = servers.cend(); it != itEnd; ++it) {
		ServerInfo* serverInfo = it->second;

		if(serverInfo->server->isListening() == true)
			info("Stopping server %s\n", it->first.c_str());

		serverInfo->server->stop();
	}

	return servers.size() > 0;
}

bool ServersManager::start(const std::string& name) {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it = servers.find(name);
	if(it != servers.end()) {
		ServerInfo* serverInfo = it->second;

		if(serverInfo->banManager)
			serverInfo->banManager->loadFile();

		if(serverInfo->server->isListening() == false) {
			info("Starting server %s on %s:%d\n", it->first.c_str(), serverInfo->listenIp->getString().c_str(), serverInfo->listenPort->getInt());

			serverInfo->server->startServer(serverInfo->listenIp->getString(),
											serverInfo->listenPort->getInt(),
											serverInfo->banManager);
		} else {
			info("Server %s already started\n", it->first.c_str());
		}

		return true;
	}

	return false;
}

bool ServersManager::stop(const std::string& name) {
	std::unordered_map<std::string, ServerInfo*>::const_iterator it = servers.find(name);
	if(it != servers.end()) {
		ServerInfo* serverInfo = it->second;

		if(serverInfo->server->isListening() == true)
			info("Stopping server %s\n", it->first.c_str());

		serverInfo->server->stop();

		return true;
	}

	return false;
}
