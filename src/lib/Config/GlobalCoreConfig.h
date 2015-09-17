#ifndef GLOBALCORECONFIG_H
#define GLOBALCORECONFIG_H

#include "ConfigInfo.h"
#include "Core/Utils.h"

#define CONFIG_FILE_KEY "configfile"

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

struct RZU_EXTERN GlobalCoreConfig {

	struct App {
		cval<std::string> &appName, &configfile;
		cval<bool> &useTcpNoDelay;
		cval<bool> &showHiddenConfig;
		cval<std::string> &encoding;

		App() :
			appName(CFG_CREATE("core.appname", Utils::getApplicationName())),
			configfile(CFG_CREATE(CONFIG_FILE_KEY, std::string(Utils::getApplicationName()) + ".opt")),
			useTcpNoDelay(CFG_CREATE("core.usetcpnodelay", false)),
			showHiddenConfig(CFG_CREATE("core.config.showhidden", false)),
			encoding(CFG_CREATE("core.encoding", "CP1252"))
		{
			Utils::autoSetAbsoluteDir(configfile);
		}
	} app;

	struct AdminConfig {
		ListenerConfig listener;
		cval<int> &dumpMode;

		AdminConfig() :
			listener("admin.console", "127.0.0.1", 4501, true, 0),
			dumpMode(CFG_CREATE("admin.dump_mode", 0)) //1: no dump, anything else: create dump on crash
		{}
	} admin;

	struct Log : public IListener {
		cval<bool> &enable;
		cval<std::string> &dir, &file, &level, &consoleLevel;
		cval<int> &maxQueueSize;

		Log() :
			enable(CFG_CREATE("core.log.enable", true)),
			dir(CFG_CREATE("core.log.dir", "log")),
			file(CFG_CREATE("core.log.file", CFG_GET("core.appname")->getString() + ".log")),
			level(CFG_CREATE("core.log.level", "info")),
			consoleLevel(CFG_CREATE("core.log.consolelevel", "info")),
			maxQueueSize(CFG_CREATE("core.log.maxqueuesize", 10000))
		{
			Utils::autoSetAbsoluteDir(dir);
			level.addListener(this, &updateConsoleLevel);
		}

		static void updateConsoleLevel(IListener* instance);
	} log;

	struct Statistics {
		cstatval<int>& connectionCount, &disconnectionCount;

		Statistics() :
			connectionCount(CFG_STAT_CREATE("stats.connections", 0)),
			disconnectionCount(CFG_STAT_CREATE("stats.disconnections", 0)) {}
	} stats;

	static GlobalCoreConfig* get();
	static void init();
};

#endif // GLOBALCORECONFIG_H
