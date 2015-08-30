#include "ConsoleSession.h"
#include "NetSession/ServersManager.h"
#include "Config/ConfigInfo.h"
#include <stdlib.h>
#include "ClassCounter.h"
#include "Database/DbConnectionPool.h"
#include "Core/CrashHandler.h"
#include <string.h>
#include "Core/Utils.h"
#include "Config/GlobalCoreConfig.h"
#include "ConsoleCommands.h"


static const char MSG_HELP[] =
		"Available commands:\r\n"
		"- start <server_name>     Start the server <server_name>. Servers names are listed when starting. If <server_name> is \"all\", all server with autostart on are started.\r\n"
		"- stop <server_name>      Same as start <server_name> but stop the server. \"stop all\" will stop all servers and exit.\r\n"
		"- set <variable> <value>  Set config variable <variable> to <value>. Double-quotes are allowed for values with spaces. Use \"\" for a escaped double quote character.\r\n"
		"- get <variable>          Get config variable value.\r\n"
		"- list                    List all connected gameservers and information about them.\r\n"
		"- mem                     List object counts.\r\n"
		"- closedb                 Close all idle database connections (use this to bring a database offline).\r\n"
		"\r\n";


#ifdef __GLIBC__
#include <malloc.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <crtdbg.h>
#include <malloc.h>
#endif

static const char MSG_UNKNOWN_SERVER_NAME[] = "Unknown server name\r\n";
static const char MSG_START_OK[] = "Server started\r\n";
static const char MSG_STOP_OK[] = "Server stopped\r\n";
static const char MSG_UNKNOWN_VARIABLE[] = "Unknown variable name\r\n";

ConsoleSession::ConsoleSession() : consoleCommands(ConsoleCommands::get()) {
	Object::info("New console session\n");
}

ConsoleSession::~ConsoleSession() {
	Object::info("Console session closed\n");
}

void ConsoleSession::write(const char *message) {
	TelnetSession::write(message, strlen(message));
}

void ConsoleSession::writef(const char *format, ...) {
	std::string str;
	va_list args;

	va_start(args, format);
	Utils::stringFormat(str, format, args);
	va_end(args);

	TelnetSession::write(str.c_str(), str.size());
}

void ConsoleSession::log(const char *message, ...) {
	va_list args;
	va_start(args, message);
	Object::log(Log::LL_Info, message, args);
	va_end(args);
}

void ConsoleSession::onConnected() {
	writef("%s - Administration console - Type \"help\" for a list of available commands\r\n",
		   CONFIG_GET()->app.appName.get().c_str());
	printPrompt();
}

void ConsoleSession::printPrompt() {
	if(getStream() && getStream()->getState() == Stream::ConnectedState)
		write("> ");
}

void ConsoleSession::onCommand(const std::vector<std::string> &args) {
	if(args.empty()) {
		printPrompt();
		return;
	}

	const std::string& commandName = args[0];
	std::vector<std::string> commandArgs(args.begin()+1, args.end());

	const ConsoleCommands::Command* command = consoleCommands->getCommand(commandName);

	if(!command) {
		writef("Unknown command : %s\r\n", commandName.c_str());
	} else {
		ConsoleCommands::Command::CallStatus status = command->call(this, commandArgs);
		switch(status) {
			case ConsoleCommands::Command::CS_Success:
				break;

			case ConsoleCommands::Command::CS_NotEnoughArgs:
				write("Not enough arguments\r\n");
				break;

			case ConsoleCommands::Command::CS_TooMuchArgs:
				write("Too many arguments\r\n");
				break;
		};

		if(status != ConsoleCommands::Command::CS_Success) {
			writef("Usage:\r\n%s\r\n", command->usageExample.c_str());
		}
	}

	printPrompt();
}
