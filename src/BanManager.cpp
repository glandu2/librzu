#include "BanManager.h"
#include <stdio.h>
#include "RappelzLibConfig.h"
#include "uv.h"

BanManager::BanManager()
{
}

void BanManager::loadFile() {
	std::string banFileName = CONFIG_GET()->ban.banFile.get();

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

//bool BanManager::stringToIpNetwork(const char* ip, uint32_t *out) {
//	int a, b, c, d;

//	if(sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
//		return false;

//	//host order -> network order
//	*out = a << 24;
//	*out |= b << 16;
//	*out |= c << 8;
//	*out |= d;
//	*out = htonl(*out);

//	return true;
//}

//void BanManager::ipNetworkToString(char ipStr[16], uint32_t ip) {
//	int a, b, c, d;

//	//host order -> network order
//	uint32_t ipLE = htonl(ip);
//	a = (ipLE >> 24) & 0xFF;
//	b = (ipLE >> 16) & 0xFF;
//	c = (ipLE >> 8) & 0xFF;
//	d = ipLE & 0xFF;

//	sprintf(ipStr, "%d.%d.%d.%d", a, b, c, d);
//}
