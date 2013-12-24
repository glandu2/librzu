#include "Log.h"
#include "private/RappelzLibConfig.h"

Log* Log::get() {
	static Log log;
	return &log;
}

Log::Log()
{
}


/*void packet(const TS_MESSAGE* message);
void log(Level level, const char* message, ...);*/
