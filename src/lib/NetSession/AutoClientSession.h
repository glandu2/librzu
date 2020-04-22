#pragma once

#include "Core/Timer.h"
#include "NetSession/ClientAuthSession.h"
#include "NetSession/ClientGameSession.h"
#include "Packet/GameTypes.h"
#include <string>
#include <time.h>
#include <vector>

struct TS_SC_CHARACTER_LIST;
struct TS_SC_LOGIN_RESULT;
struct TS_TIMESYNC;
struct TS_SC_GAME_TIME;
struct TS_SC_SET_TIME;

class AutoClientSession;

class RZU_EXTERN AutoAuthSession : public ClientAuthSession {
public:
	AutoAuthSession(AutoClientSession* gameSession,
	                const std::string& ip,
	                uint16_t port,
	                const std::string& account,
	                const std::string& password,
	                int serverIdx,
	                int delayTime,
	                int ggRecoTime,
	                int epic);

	void connect();
	void delayedConnect();

private:
	virtual void onAuthDisconnected(bool causedByRemote) override;
	virtual void onAuthResult(TS_ResultCode result, const std::string& resultString) override;
	virtual void onServerList(const std::vector<ServerInfo>& servers, uint16_t lastSelectedServerId) override;

	virtual void onGameDisconnected(bool causedByRemote) override;
	virtual void onGameResult(TS_ResultCode result) override;

	void onDelayRecoExpired();
	void onGGTimerExpired();

private:
	AutoClientSession* gameSession;
	std::string ip;
	uint16_t port;
	std::string account;
	std::string password;
	int serverIdx;

	int delayTime;
	int ggRecoTime;

	Timer<AutoAuthSession> delayRecoTimer;
	Timer<AutoAuthSession> ggRecoTimer;
};

class RZU_EXTERN AutoClientSession : public ClientGameSession {
public:
	explicit AutoClientSession(const std::string& ip,
	                           uint16_t port,
	                           const std::string& account,
	                           const std::string& password,
	                           int serverIdx,
	                           const std::string& playername,
	                           int epic = EPIC_LATEST,
	                           bool manageTimesync = true,
	                           int delayTime = 5000,
	                           int ggRecoTime = 280);

	void connect();

	ar_time_t getGameTime() { return ar_time_t{getLocalGameTime().get() + gameTimeOffset}; }
	time_t getEpochTime() { return time(nullptr) + epochTimeOffset; }
	const std::string& getPlayerName() { return playername; }
	bool isConnected() { return connectedInGame; }
	ar_handle_t getLocalPlayerHandle() { return handle; }

protected:
	template<class T> struct EventTag {};

	virtual void onConnected(EventTag<AutoClientSession> tag = {}) {}
	virtual void onPacketReceived(const TS_MESSAGE* packet, EventTag<AutoClientSession> tag = {}) {}
	virtual void onDisconnected(EventTag<AutoClientSession> tag = {}) {}

private:
	friend class AutoAuthSession;
	bool onAuthDisconnected(bool causedByRemote) { return true; }

	void setConnected(bool connected);

	virtual void onGameConnected() override final;
	virtual void onGamePacketReceived(const TS_MESSAGE* packet) override final;
	virtual void onGameDisconnected(bool causedByRemote) override final;

	// Prevent using direct connect
	using ClientGameSession::connect;

private:
	void onUpdatePacketExpired();

	void onCharacterList(const TS_SC_CHARACTER_LIST* packet);
	void onCharacterLoginResult(const TS_SC_LOGIN_RESULT* packet);
	void onTimeSync(const TS_TIMESYNC* packet);
	void onSetTime(const TS_SC_SET_TIME* packet);
	void onGameTime(const TS_SC_GAME_TIME* packet);

	ar_time_t getLocalGameTime();

private:
	AutoAuthSession authSession;
	bool manageTimesync;

	std::string playername;
	bool connectedInGame;

	ar_handle_t handle;
	int32_t gameTimeOffset;
	int32_t epochTimeOffset;

	Timer<AutoClientSession> updateTimer;
};

