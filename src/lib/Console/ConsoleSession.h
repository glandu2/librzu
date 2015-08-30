#ifndef CONSOLESESSION_H
#define CONSOLESESSION_H

#include "NetSession/TelnetSession.h"
#include "IWritableConsole.h"

class ConsoleCommands;

class RZU_EXTERN ConsoleSession : public TelnetSession, public IWritableConsole
{
	DECLARE_CLASS(ConsoleSession)
public:
	ConsoleSession();
	virtual ~ConsoleSession();

	virtual void write(const char *message) override;
	virtual void writef(const char *format, ...) override;
	virtual void log(const char *message, ...) override;

protected:
	void onConnected();
	void printPrompt();
	void onCommand(const std::vector<std::string>& args);

private:
	const ConsoleCommands* consoleCommands;
};

#endif // CONSOLESESSION_H
