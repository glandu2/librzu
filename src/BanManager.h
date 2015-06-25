#ifndef BANMANAGER_H
#define BANMANAGER_H

#include "Object.h"
#include <unordered_set>
#include <stdint.h>

class RZU_EXTERN BanManager : public Object
{
	DECLARE_CLASS(BanManager)

public:
	BanManager();

	void loadFile();
	bool isBanned(uint32_t ip);

	static void registerConfig();
private:
	std::unordered_set<uint32_t> bannedIps;
};

#endif // BANMANAGER_H
