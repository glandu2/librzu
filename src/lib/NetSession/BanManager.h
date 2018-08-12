#ifndef BANMANAGER_H
#define BANMANAGER_H

#include "Core/Object.h"
#include "Core/Timer.h"
#include "Stream/StreamAddress.h"
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>

class RZU_EXTERN BanManager : public Object {
	DECLARE_CLASS(BanManager)
public:
	BanManager();

	void loadFile();
	bool checkAcceptNewClient(StreamAddress ip);
	void closedClient(StreamAddress ip);
	void banIp(StreamAddress ip);

	static void registerConfig();

protected:
	void onTimerExpire();

private:
	int maxConnectionPerDayPerIp;
	int maxClientPerIp;
	std::unordered_set<StreamAddress, StreamAddressHasher> bannedIps;

	struct IpData {
		int32_t connectedClients = 0;
		int32_t connectionThisDay = 0;
	};
	std::unordered_map<StreamAddress, IpData, StreamAddressHasher> ipDatas;
	Timer<BanManager> resetConnectionsThisDayTimer;
};

#endif  // BANMANAGER_H
