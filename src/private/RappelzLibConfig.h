#ifndef RAPPELZLIBCONFIG_H
#define RAPPELZLIBCONFIG_H

#include "ConfigInfo.h"

struct RappelzLibConfig
{
	struct App {
		std::string& appName;

		App() : appName(CFG("core.appname", "Rappelz Auth Server")) {}
	} app;

	struct Log {
		bool& enable;
		std::string& dir, &file;
		int& level; //Level 0 = fatal, Level max (5) = trace

		Log() :
			enable(CFG("core.log.enable", false)),
			dir(CFG("core.log.dir", "log")),
			file(CFG("core.log.file", CFG("core.appname", "auth"))),
			level(CFG("core.log.level", 1)) {}
	} log;

	struct TrafficDump {
		bool& enable;
		std::string& path;

		TrafficDump() :
			enable(CFG("core.trafficdump.enable", false)),
			path(CFG("core.trafficdump.path", "auth.traffic.log")) {}
	} trafficDump;

	static RappelzLibConfig* get();
};

#define CONFIG_GET() RappelzLibConfig::get()

#endif // RAPPELZLIBCONFIG_H
