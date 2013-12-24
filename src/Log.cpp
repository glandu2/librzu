#include "Log.h"
#include "private/RappelzLibConfig.h"

Log* Log::get() {
	static Log log(CONFIG_GET()->log.dir + CONFIG_GET()->log.file);
	return &log;
}

Log* Log::getPacket() {
	static Log log(CONFIG_GET()->trafficDump.path);
	return &log;
}

Log::Log(const std::string& file)
{
}

void Log::log(Level level, const char* message, ...) {

}
