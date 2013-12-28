#include "Log.h"
#include "private/RappelzLibConfig.h"

Log* Log::get() {
	static Log log(CONFIG_GET()->log.enable, (Level)CONFIG_GET()->log.level, CONFIG_GET()->log.dir + CONFIG_GET()->log.file);
	return &log;
}

Log* Log::getPacket() {
	static Log log(CONFIG_GET()->trafficDump.enable, LL_Trace, CONFIG_GET()->trafficDump.path);
	return &log;
}

Log::Log(const bool& enabled, const Level& maxLevel, const std::string& file) :
	enabled(enabled),
	maxLevel(maxLevel),
	filename(file)
{
}

void Log::log(Level level, const char* message, ...) {

}
