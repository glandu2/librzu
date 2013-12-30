#include "Log.h"
#include "private/RappelzLibConfig.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#ifdef _MSC_VER
#define va_copy(d,s) ((d) = (s))
#endif

static const char * const  LEVELSTRINGS[] = { "FATAL", "ERROR", "Warn", "Info", "Debug", "Trace" };

Log* Log::messageLogger = nullptr;
Log* Log::packetLogger = nullptr;

bool Log::init() {
	messageLogger = new Log(CONFIG_GET()->log.enable, CONFIG_GET()->log.level, CONFIG_GET()->log.dir, CONFIG_GET()->log.file);
	packetLogger = new Log(CONFIG_GET()->trafficDump.enable, LL_Trace, CONFIG_GET()->trafficDump.dir, CONFIG_GET()->trafficDump.file);
	return true;
}

void Log::updateLevel(void* instance, cval<std::string>* level) {
	Log* thisInstance = (Log*) instance;

	if(level->get() == "fatal")
		thisInstance->maxLevel = LL_Fatal;
	else if(level->get() == "error")
		thisInstance->maxLevel = LL_Error;
	else if(level->get() == "warning")
		thisInstance->maxLevel = LL_Warning;
	else if(level->get() == "info")
		thisInstance->maxLevel = LL_Info;
	else if(level->get() == "debug")
		thisInstance->maxLevel = LL_Debug;
	else if(level->get() == "trace")
		thisInstance->maxLevel = LL_Trace;
	else {
		thisInstance->error("Invalid level value: %s. Using warning.\n", level->get().c_str());
		thisInstance->maxLevel = LL_Warning;
	}

	thisInstance->debug("Using log level %s\n", LEVELSTRINGS[thisInstance->maxLevel]);
}

void Log::updateFile(void* instance, cval<std::string>* str) {
	Log* thisInstance = (Log*) instance;
	FILE* newFile;
	std::string newFileName = thisInstance->dir.get() + "/" + thisInstance->fileName.get();

	newFile = fopen(newFileName.c_str(), "ab");
	if(newFile == nullptr) {
		if(thisInstance->file)
			thisInstance->error("Couldnt open new log file %s, keeping old one\n", newFileName.c_str());
		else {
			thisInstance->error("Couldnt open log file %s for writting !\n", newFileName.c_str());
		}
	} else {
		if(thisInstance->file) {
			thisInstance->info("Switching log file to %s\n", newFileName.c_str());

			uv_mutex_lock(&thisInstance->lock);

			fclose((FILE*)thisInstance->file);
			thisInstance->file = newFile;

			uv_mutex_unlock(&thisInstance->lock);
		} else {
			uv_mutex_lock(&thisInstance->lock);
			thisInstance->file = newFile;
			uv_mutex_unlock(&thisInstance->lock);
		}
		thisInstance->log(LL_Info, thisInstance->getObjectName(), "Log file \"%s\" opened\n", newFileName.c_str());
	}
}

Log::Log(cval<bool>& enabled, cval<std::string>& maxLevel, cval<std::string>& dir, cval<std::string>& fileName) :
	enabled(enabled),
	dir(dir),
	fileName(fileName),
	file(nullptr)
{
	uv_mutex_init(&lock);
	updateLevel(this, &maxLevel);
	updateFile(this, &fileName);

	maxLevel.addListener(this, &updateLevel);
	dir.addListener(this, &updateFile);
	fileName.addListener(this, &updateFile);
}

Log::Log(cval<bool>& enabled, Level maxLevel, cval<std::string>& dir, cval<std::string>& fileName) :
	enabled(enabled),
	maxLevel(maxLevel),
	dir(dir),
	fileName(fileName),
	file(nullptr)
{
	uv_mutex_init(&lock);
	updateFile(this, &fileName);

	dir.addListener(this, &updateFile);
	fileName.addListener(this, &updateFile);
}

Log::~Log() {
	info("Log file closed\n");
	if(file)
		fclose((FILE*)file);
}

void Log::log(Level level, const char *objectName, const char* message, ...) {
	va_list args;

	if(level > maxLevel)
		return;

	va_start(args, message);
	log(level, objectName, message, args);
	va_end(args);
}

void Log::log(Level level, const char *objectName, const char* message, va_list args) {
	if(level > maxLevel)
		return;

	struct tm *localtm;
	time_t curtime = time(NULL);
	va_list argsConsole;
	va_copy(argsConsole, args);

	uv_mutex_lock(&lock);

	//brktimegm(time(NULL), &localtm);
	localtm = gmtime(&curtime);

	fprintf(stderr, "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm->tm_year+1900, localtm->tm_mon+1, localtm->tm_mday, localtm->tm_hour, localtm->tm_min, localtm->tm_sec, LEVELSTRINGS[level], objectName);

	vfprintf(stderr, message, argsConsole);

	if(file && enabled.get()) {
		fprintf((FILE*)file, "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm->tm_year+1900, localtm->tm_mon+1, localtm->tm_mday, localtm->tm_hour, localtm->tm_min, localtm->tm_sec, LEVELSTRINGS[level], objectName);

		vfprintf((FILE*)file, message, args);

		fflush((FILE*)file);
	}

	uv_mutex_unlock(&lock);
}
