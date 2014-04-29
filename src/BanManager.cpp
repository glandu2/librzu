#include "BanManager.h"
#include <stdio.h>
#include "RappelzLibConfig.h"

BanManager::BanManager()
{
}

void BanManager::loadFile() {
	FILE* file = fopen(CONFIG_GET()->ban.banFile.get().c_str(), "rb");
	if(!file) {
		debug("No ban ip file\n");
		return;
	}

	char line[512];

	while(fgets(line, 512, file)) {
		char *p = line;
		while(*p && (isdigit(*p) || *p == '.'))
			p++;
		*p = '\0';

		//If not empty string
		if(line[0]) {
			uint32_t inet;
			if(stringToIpNetwork(line, &inet)) {
				bannedIps.insert(inet);
				info("Add banned ip: %s\n", line);
			}
		}
	}

	fclose(file);
}

bool BanManager::isBanned(uint32_t ip) {
	if(bannedIps.find(ip) != bannedIps.end())
		return true;

	return false;
}

bool BanManager::stringToIpNetwork(const char* ip, uint32_t *out) {
	int a, b, c, d;

	if(sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
		return false;

	//host order -> network order
	*out = a << 24;
	*out |= b << 16;
	*out |= c << 8;
	*out |= d;
	*out = htonl(*out);

	return true;
}
