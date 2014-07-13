#ifndef BANNEDIP_H
#define BANNEDIP_H

#include "Object.h"
#include <unordered_set>
#include <string>
#include <stdint.h>

class RAPPELZLIB_EXTERN BanManager : public Object
{
	DECLARE_CLASS(BanManager)

public:
	BanManager();

	void loadFile();
	bool isBanned(uint32_t ip);

protected:
//	bool stringToIpNetwork(const char* ip, uint32_t* out);
//	void ipNetworkToString(char ipStr[16], uint32_t ip);

private:
	std::unordered_set<uint32_t> bannedIps;
};

#endif // BANNEDIP_H
