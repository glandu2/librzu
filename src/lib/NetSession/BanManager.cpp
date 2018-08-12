#include "BanManager.h"
#include "Config/GlobalCoreConfig.h"
#include "uv.h"
#include <assert.h>
#include <memory>
#include <stdio.h>
#include <string>

struct BanConfig {
	cval<std::string>& banFile;
	cval<int>& maxClientPerIp;
	cval<int>& maxConnectionPerDayPerIp;

	BanConfig()
	    : banFile(CFG_CREATE("ban.ipfile", "bannedip.txt")),
	      maxClientPerIp(CFG_CREATE("ban.maxclientperip", 0)),
	      maxConnectionPerDayPerIp(CFG_CREATE("ban.maxconnectionperdayperip", 0)) {
		Utils::autoSetAbsoluteDir(banFile);
	}
};
static std::unique_ptr<BanConfig> config;

void BanManager::registerConfig() {
	if(!config)
		config.reset(new BanConfig);
}

void BanManager::onTimerExpire() {
	log(LL_Info, "Reseting connections per day per ip\n");
	for(auto& ipData : ipDatas) {
		ipData.second.connectionThisDay = 0;
	}
}

BanManager::BanManager() {
	assert(config != nullptr);
	maxConnectionPerDayPerIp = config->maxConnectionPerDayPerIp.get();
	maxClientPerIp = config->maxClientPerIp.get();
	resetConnectionsThisDayTimer.start(this, &BanManager::onTimerExpire, 24 * 60 * 60 * 1000, 24 * 60 * 60 * 1000);
	resetConnectionsThisDayTimer.unref();
}

void BanManager::loadFile() {
	std::string banFileName = config->banFile.get();

	// Cache config
	maxConnectionPerDayPerIp = config->maxConnectionPerDayPerIp.get();
	maxClientPerIp = config->maxClientPerIp.get();

	FILE* file = fopen(banFileName.c_str(), "rb");
	if(!file) {
		log(LL_Debug, "No ban ip file: \"%s\"\n", banFileName.c_str());
		return;
	}

	char line[512];

	std::unordered_set<StreamAddress, StreamAddressHasher> newBannedIps;

	while(fgets(line, 512, file)) {
		bool isIpv6 = false;
		char* p = line;
		while(*p && (Utils::ishex(*p) || *p == '.' || *p == ':')) {
			if(*p == ':')
				isIpv6 = true;
			p++;
		}
		*p = '\0';

		// If not empty string
		if(line[0]) {
			StreamAddress inet;
			inet.port = 0;
			if(inet.setFromName(line) >= 0) {
				newBannedIps.insert(inet);
				if(bannedIps.erase(inet) == 0) {
					char realIp[INET6_ADDRSTRLEN];
					inet.getName(realIp, sizeof(realIp));
					log(LL_Info, "Add banned ip: %s\n", realIp);
				}
			}
		}
	}

	std::unordered_set<StreamAddress, StreamAddressHasher>::const_iterator it, itEnd;
	char ipStr[INET6_ADDRSTRLEN];
	for(it = bannedIps.cbegin(), itEnd = bannedIps.cend(); it != itEnd; ++it) {
		const StreamAddress& inet = *it;
		inet.getName(ipStr, sizeof(ipStr));
		log(LL_Info, "Removed banned ip: %s\n", ipStr);
	}

	bannedIps.swap(newBannedIps);

	fclose(file);
}

bool BanManager::checkAcceptNewClient(StreamAddress ip) {
	ip.port = 0;  // Port is not used to check bans

	char ipStr[INET6_ADDRSTRLEN];
	ip.getName(ipStr, sizeof(ipStr));

	if(bannedIps.find(ip) != bannedIps.end()) {
		log(LL_Debug, "Kick banned ip %s\n", ipStr);
		return false;
	}

	if(maxConnectionPerDayPerIp == 0 && maxClientPerIp == 0)
		return true;

	IpData& ipData = ipDatas[ip];

	if(maxConnectionPerDayPerIp > 0 && ipData.connectionThisDay >= maxConnectionPerDayPerIp) {
		bannedIps.insert(ip);
		log(LL_Warning,
		    "Too many connections per day from ip %s: %d, now banned until server restart\n",
		    ipStr,
		    ipData.connectionThisDay);
		return false;
	}

	ipData.connectionThisDay++;

	if(maxClientPerIp > 0 && ipData.connectedClients >= maxClientPerIp) {
		log(LL_Debug, "Too many connections at once from ip %s: %d\n", ipStr, ipData.connectedClients);
		return false;
	}

	ipData.connectedClients++;

	return true;
}

void BanManager::closedClient(StreamAddress ip) {
	ip.port = 0;  // Port is not used to check bans

	auto it = ipDatas.find(ip);
	if(it != ipDatas.end()) {
		IpData& ipData = it->second;
		if(ipData.connectedClients > 0)
			ipData.connectedClients--;
		else {
			char ipStr[INET6_ADDRSTRLEN];
			ip.getName(ipStr, sizeof(ipStr));
			log(LL_Warning, "IP %s has more disconnection than connection\n", ipStr);
		}
	}
}

void BanManager::banIp(StreamAddress ip) {
	char ipStr[INET6_ADDRSTRLEN];
	ip.getName(ipStr, sizeof(ipStr));

	bannedIps.insert(ip);
	log(LL_Warning, "Banned ip %s\n", ipStr);
}
