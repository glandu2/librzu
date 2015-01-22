#ifndef RAPPELZLIBCONFIG_H
#define RAPPELZLIBCONFIG_H

#include "ConfigInfo.h"
#include "Utils.h"

#define CONFIG_FILE_KEY "configfile"

struct RAPPELZLIB_EXTERN GlobalCoreConfig {

	struct App {
		cval<std::string> &appName, &configfile;
		cval<bool> &useTcpNoDelay;

		App() :
			appName(CFG_CREATE("core.appname", "RappelzEmu")),
			configfile(CFG_CREATE(CONFIG_FILE_KEY, "rappelzemu.opt")),
			useTcpNoDelay(CFG_CREATE("core.usetcpnodelay", false))
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
			maxQueueSize(CFG_CREATE("core.log.maxqueuesize", 1000))
		{
			Utils::autoSetAbsoluteDir(dir);
			level.addListener(this, &updateConsoleLevel);
		}

		static void updateConsoleLevel(IListener* instance);
	} log;

	struct Ban {
		//DbConfig dbBan;
		cval<std::string> &banFile;

		Ban() :
			//dbBan("ban."),
			banFile(CFG_CREATE("ban.ipfile", "bannedip.txt"))
		{
			Utils::autoSetAbsoluteDir(banFile);
		}
	} ban;


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

#endif // RAPPELZLIBCONFIG_H
