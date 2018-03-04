#ifndef CONSOLESESSION_H
#define CONSOLESESSION_H

#include "IWritableConsole.h"
#include "NetSession/SessionServer.h"
#include "NetSession/TelnetSession.h"

class ConsoleCommands;
class ServersManager;

class RZU_EXTERN ConsoleSession : public TelnetSession, public IWritableConsole {
	DECLARE_CLASS(ConsoleSession)
public:
	ConsoleSession();
	virtual ~ConsoleSession();

	using TelnetSession::write;
	virtual void write(const char* message) override;
	virtual void writef(const char* format, ...) override;
	virtual void log(const char* message, ...) override;

protected:
	EventChain<SocketSession> onConnected();
	void printPrompt();
	void onCommand(const std::vector<std::string>& args);

private:
	const ConsoleCommands* consoleCommands;
};

class RZU_EXTERN ConsoleServer : public SessionServer<ConsoleSession> {
public:
	ConsoleServer(ServersManager* serverManager);
};

#endif  // CONSOLESESSION_H
