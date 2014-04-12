#include "Log.h"
#include "RappelzLibConfig.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "Utils.h"
#include "EventLoop.h"

static const char * const  LEVELSTRINGS[] = { "FATAL", "ERROR", "Warn", "Info", "Debug", "Trace" };

Log* Log::messageLogger = nullptr;
Log* Log::packetLogger = nullptr;

bool Log::init() {
	messageLogger = new Log(CONFIG_GET()->log.enable, CONFIG_GET()->log.level, CONFIG_GET()->log.consoleLevel, CONFIG_GET()->log.dir, CONFIG_GET()->log.file);
	packetLogger = new Log(CONFIG_GET()->trafficDump.enable, LL_Trace, LL_Error, CONFIG_GET()->trafficDump.dir, CONFIG_GET()->trafficDump.file);
	return true;
}

Log::Log(cval<bool>& enabled, cval<std::string>& fileMaxLevel, cval<std::string>& consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName) :
	fileMaxLevel(LL_Info),
	consoleMaxLevel(LL_Info),
	dir(dir),
	fileName(fileName),
	file(nullptr)
{
	uv_mutex_init(&lock);
	updateEnabled(this, &enabled);
	updateFileLevel(this, &fileMaxLevel);
	updateConsoleLevel(this, &consoleMaxLevel);

	enabled.addListener(this, &updateEnabled);
	fileMaxLevel.addListener(this, &updateFileLevel);
	consoleMaxLevel.addListener(this, &updateConsoleLevel);
	dir.addListener(this, &updateFile);
	fileName.addListener(this, &updateFile);

	uv_timer_init(EventLoop::getLoop(), &flushTimer);
	flushTimer.data = this;
	uv_unref((uv_handle_t*)&flushTimer);
	uv_timer_start(&flushTimer, &flushLogFile, 5000, 5000);
}

Log::Log(cval<bool>& enabled, Level fileMaxLevel, Level consoleMaxLevel, cval<std::string>& dir, cval<std::string>& fileName) :
	fileMaxLevel(LL_Info),
	consoleMaxLevel(LL_Info),
	dir(dir),
	fileName(fileName),
	file(nullptr)
{
	uv_mutex_init(&lock);
	updateEnabled(this, &enabled);
	this->fileMaxLevel = fileMaxLevel;
	this->consoleMaxLevel = consoleMaxLevel;

	enabled.addListener(this, &updateEnabled);
	dir.addListener(this, &updateFile);
	fileName.addListener(this, &updateFile);

	uv_timer_init(EventLoop::getLoop(), &flushTimer);
	flushTimer.data = this;
	uv_unref((uv_handle_t*)&flushTimer);
	uv_timer_start(&flushTimer, &flushLogFile, 5000, 5000);
}

Log::~Log() {
	close();
}

void Log::updateEnabled(IListener* instance, cval<bool>* enable) {
	Log* thisInstance = (Log*) instance;

	if(!thisInstance->file && enable->get()) {
		thisInstance->open();
	} else if(thisInstance->file && !enable->get()) {
		thisInstance->close();
	}
}

void Log::updateFileLevel(IListener* instance, cval<std::string>* level) {
	Log* thisInstance = (Log*) instance;

	thisInstance->updateLevel(false, level->get());
}

void Log::updateConsoleLevel(IListener* instance, cval<std::string>* level) {
	Log* thisInstance = (Log*) instance;

	thisInstance->updateLevel(true, level->get());
}

void Log::updateLevel(bool isConsole, const std::string& level) {
	Level *levelToChange;

	if(isConsole)
		levelToChange = &consoleMaxLevel;
	else
		levelToChange = &fileMaxLevel;

	if(level == "fatal")
		*levelToChange = LL_Fatal;
	else if(level == "error")
		*levelToChange = LL_Error;
	else if(level == "warning")
		*levelToChange = LL_Warning;
	else if(level == "info")
		*levelToChange = LL_Info;
	else if(level == "debug")
		*levelToChange = LL_Debug;
	else if(level == "trace")
		*levelToChange = LL_Trace;
	else {
		error("Invalid level value: %s. Using warning.\n", level.c_str());
		*levelToChange = LL_Warning;
	}

	if(isConsole)
		debug("Using console log level %s\n", LEVELSTRINGS[*levelToChange]);
	else
		debug("Using file log level %s\n", LEVELSTRINGS[*levelToChange]);
}

void Log::updateFile(IListener* instance, cval<std::string>* str) {
	Log* thisInstance = (Log*) instance;

	thisInstance->open();
}

bool Log::open() {
	std::string absoluteDir = dir.get();
	std::string newFileName = absoluteDir + "/" + fileName.get();
	FILE* newfile;

	Utils::mkdir(absoluteDir.c_str());

	newfile = fopen(newFileName.c_str(), "ab");
	if(!newfile) {
		if(file)
			error("Couldnt open new log file %s, keeping old one\n", newFileName.c_str());
		else
			error("Couldnt open log file %s for writting !\n", newFileName.c_str());
		return false;
	} else {
		if(file) {
			info("Switching log file to %s\n", newFileName.c_str());

			uv_mutex_lock(&lock);

			fclose((FILE*)file);
			file = newfile;

			uv_mutex_unlock(&lock);
		} else {
			uv_mutex_lock(&lock);
			file = newfile;
			uv_mutex_unlock(&lock);
		}
		log(LL_Info, getObjectName(), getObjectNameSize(), "Log file \"%s\" opened\n", newFileName.c_str());
		return true;
	}
}

void Log::close() {
	info("Closing log file\n");
	uv_mutex_lock(&lock);
	if(file)
		fclose((FILE*)file);
	file = nullptr;
	uv_mutex_unlock(&lock);
}

void Log::log(Level level, const char *objectName, size_t objectNameSize, const char* message, ...) {
	va_list args;

	if(level > fileMaxLevel && level > consoleMaxLevel)
		return;

	va_start(args, message);
	log(level, objectName, objectNameSize, message, args);
	va_end(args);
}

void Log::log(Level level, const char *objectName, size_t objectNameSize, const char* message, va_list args) {
	if(level > fileMaxLevel && level > consoleMaxLevel)
		return;

	struct tm localtm;
	va_list argsConsole;
	va_copy(argsConsole, args);

	uv_mutex_lock(&lock);

	Utils::getGmTime(time(NULL), &localtm);

	//26 char to %-5s included
	size_t bufferSize = 26 + objectNameSize + 3;
	char *buffer = (char*) alloca(bufferSize);
	size_t strLen = snprintf(buffer, bufferSize, "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm.tm_year, localtm.tm_mon, localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec, LEVELSTRINGS[level], objectName);
	if(strLen >= bufferSize) {
		fprintf(stderr, "------------------- ERROR Log::log: Log buffer was too small, log source might be truncated\n");
		strLen = bufferSize-1;
	}

	if(level <= consoleMaxLevel) {
		fwrite(buffer, 1, strLen, stderr);
		vfprintf(stderr, message, argsConsole);
	}

	if(file && level <= fileMaxLevel) {
		fwrite(buffer, 1, strLen, (FILE*)file);
		vfprintf((FILE*)file, message, args);
	}

	uv_mutex_unlock(&lock);
}

void Log::flushLog() {
	if(uv_mutex_trylock(&lock) == 0) {
		if(file) {
			fflush((FILE*)file);
		}
		uv_mutex_unlock(&lock);
	}
}

void Log::flushLogFile(uv_timer_t* timer) {
	Log* thisInstance = (Log*) timer->data;
	thisInstance->flushLog();
}
