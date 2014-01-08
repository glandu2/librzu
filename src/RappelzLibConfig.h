#ifndef RAPPELZLIBCONFIG_H
#define RAPPELZLIBCONFIG_H

#include "ConfigInfo.h"
#include "Utils.h"

#define CONFIG_FILE_KEY "configfile"

struct RAPPELZLIB_EXTERN RappelzLibConfig
{
	struct App {
		cval<std::string> &appName, &configfile;

		App() :
			appName(CFG("core.appname", "RappelzEmu")),
			configfile(CFG(CONFIG_FILE_KEY, "rappelzemu.opt"))
		{
			Utils::autoSetAbsoluteDir(configfile);
		}
	} app;

	struct Log : public IListener {
		cval<bool> &enable;
		cval<std::string> &dir, &file, &level, &consoleLevel;

		Log() :
			enable(CFG("core.log.enablefile", true)),
			dir(CFG("core.log.dir", "log")),
			file(CFG("core.log.file", CFG("core.appname", "rappelzemu").get() + ".log")),
			level(CFG("core.log.level", "info")),
			consoleLevel(CFG("core.log.consolelevel", "info"))
		{
			Utils::autoSetAbsoluteDir(dir);
			level.addListener(this, &updateConsoleLevel);
		}

		static void updateConsoleLevel(IListener* instance);
	} log;

	struct TrafficDump {
		cval<bool> &enable;
		cval<std::string> &dir, &file;

		TrafficDump() :
			enable(CFG("core.trafficdump.enable", false)),
			dir(CFG("core.trafficdump.dir", "traffic_log")),
			file(CFG("core.trafficdump.file", "rappelzemu.log"))
		{
			Utils::autoSetAbsoluteDir(dir);
		}
	} trafficDump;

	static RappelzLibConfig* get();
};

#ifndef CONFIG_GET
#define CONFIG_GET() RappelzLibConfig::get()
#endif

#endif // RAPPELZLIBCONFIG_H
