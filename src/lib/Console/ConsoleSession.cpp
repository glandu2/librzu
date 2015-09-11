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
#include "../NetSession/SessionServer.h"


struct ListenerConfig {
	cval<std::string> &listenIp;
	cval<int> &port, &idleTimeout;
	cval<bool> &autoStart;

	ListenerConfig(const std::string& prefix, const char* defaultIp, int defaultPort, bool autoStart = true, int idleTimeout = 0) :
		listenIp(CFG_CREATE(prefix + ".ip", defaultIp)),
		port(CFG_CREATE(prefix + ".port", defaultPort)),
		idleTimeout(CFG_CREATE(prefix + ".idletimeout", idleTimeout)),
		autoStart(CFG_CREATE(prefix + ".autostart", autoStart))
	{}
};

struct AdminConfig {
	ListenerConfig listener;

	AdminConfig() :
		listener("admin.console", "127.0.0.1", 4501, true, 0)
	{}
};
static AdminConfig* admin = nullptr;

void ConsoleSession::init() {
	if(!admin)
		admin = new AdminConfig;
}

void ConsoleSession::start(ServersManager* serverManager) {
	if(!admin) {
		logStatic(LL_Warning, ConsoleSession::getStaticClassName(), "init() was not called before start()\n");
		init();
	}

	static SessionServer<ConsoleSession> adminConsoleServer(
				admin->listener.listenIp,
				admin->listener.port,
				&admin->listener.idleTimeout);
	serverManager->addServer("admin.console", &adminConsoleServer, admin->listener.autoStart, true);
}

ConsoleSession::ConsoleSession() : consoleCommands(ConsoleCommands::get()) {
	Object::log(LL_Info, "New console session\n");
}

ConsoleSession::~ConsoleSession() {
	Object::log(LL_Info, "Console session closed\n");
}

void ConsoleSession::write(const char *message) {
	TelnetSession::write(message, strlen(message));
}

void ConsoleSession::writef(const char *format, ...) {
	std::string str;
	va_list args;

	va_start(args, format);
	Utils::stringFormatv(str, format, args);
	va_end(args);

	TelnetSession::write(str.c_str(), str.size());
}

void ConsoleSession::log(const char *message, ...) {
	va_list args;
	va_start(args, message);
	Object::logv(LL_Info, message, args);
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
