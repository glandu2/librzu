#include "BanManager.h"
#include <stdio.h>
#include "GlobalCoreConfig.h"
#include "uv.h"
#include <string>
#include <assert.h>

struct BanConfig {
	cval<std::string> &banFile;

	BanConfig() :
		banFile(CFG_CREATE("ban.ipfile", "bannedip.txt"))
	{
		Utils::autoSetAbsoluteDir(banFile);
	}
};
static BanConfig* config = nullptr;

void BanManager::registerConfig() {
	if(!config)
		config = new BanConfig;
}

BanManager::BanManager() {
	assert(config != nullptr);
}

void BanManager::loadFile() {
	std::string banFileName = config->banFile.get();

	FILE* file = fopen(banFileName.c_str(), "rb");
	if(!file) {
		debug("No ban ip file: \"%s\"\n", banFileName.c_str());
		return;
	}

	char line[512];
	uint32_t inet;

	std::unordered_set<uint32_t> newBannedIps;

	while(fgets(line, 512, file)) {
		char *p = line;
		while(*p && (isdigit(*p) || *p == '.'))
			p++;
		*p = '\0';

		//If not empty string
		if(line[0]) {
			if(uv_inet_pton(AF_INET, line, &inet) >= 0) {
				newBannedIps.insert(inet);
				if(bannedIps.erase(inet) == 0)
					info("Add banned ip: %s\n", line);
			}
		}
	}

	std::unordered_set<uint32_t>::const_iterator it, itEnd;
	char ipStr[INET_ADDRSTRLEN];
	for(it = bannedIps.cbegin(), itEnd = bannedIps.cend(); it != itEnd; ++it) {
		inet = *it;
		uv_inet_ntop(AF_INET, &inet, ipStr, INET_ADDRSTRLEN);
		info("Removed banned ip: %s\n", ipStr);
	}

	bannedIps.swap(newBannedIps);

	fclose(file);
}

bool BanManager::isBanned(uint32_t ip) {
	if(bannedIps.find(ip) != bannedIps.end())
		return true;

	return false;
}
