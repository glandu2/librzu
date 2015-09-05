#ifndef GLOBALCORECONFIG_H
#define GLOBALCORECONFIG_H

#include "ConfigInfo.h"
#include "Core/Utils.h"

#define CONFIG_FILE_KEY "configfile"

struct RZU_EXTERN GlobalCoreConfig {

	struct App {
		cval<std::string> &appName, &configfile;
		cval<bool> &useTcpNoDelay;
		cval<bool> &showHiddenConfig;

		App() :
			appName(CFG_CREATE("core.appname", Utils::getApplicationName())),
			configfile(CFG_CREATE(CONFIG_FILE_KEY, std::string(Utils::getApplicationName()) + ".opt")),
			useTcpNoDelay(CFG_CREATE("core.usetcpnodelay", false)),
			showHiddenConfig(CFG_CREATE("core.config.showhidden", false))
		{
			Utils::autoSetAbsoluteDir(configfile);
		}
	} app;

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

#ifndef CONFIG_GET
#define CONFIG_GET() GlobalCoreConfig::get()
#endif

#endif // GLOBALCORECONFIG_H
