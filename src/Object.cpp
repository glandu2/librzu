#include "Object.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "EventLoop.h"
#include "Log.h"
#include "Utils.h"
#include "GlobalCoreConfig.h"

Object::Object() {
	objectName = nullptr;
	objectNameSize = 0;
	dirtyName = true;
	scheduledForDelete = false;
}

Object::~Object() {
	if(objectName)
		delete[] objectName;
}

void Object::setObjectName(const char *name) {
	const char* oldName = objectName;

	if(name) {
		objectNameSize = strlen(name)+1;
		objectName = new char[objectNameSize];
		strcpy(objectName, name);
	} else {
		objectNameSize = 0;
		objectName = NULL;
	}

	dirtyName = false;

	if(oldName)
		delete[] oldName;
}

//maxLen without null terminator
void Object::setObjectName(int maxLen, const char *format, ...) {
	va_list args;
	const char* oldName = objectName;

	if(format && maxLen > 0) {
		objectNameSize = maxLen+1;
		objectName = new char[objectNameSize];
		va_start(args, format);
		vsnprintf(objectName, maxLen+1, format, args);
		va_end(args);
		objectName[maxLen] = 0;
	} else {
		objectNameSize = 0;
		objectName = NULL;
	}

	dirtyName = false;

	if(oldName)
		delete[] oldName;
}

const char *Object::getObjectName(size_t *size) {
	if(dirtyName)
		updateObjectName();
	dirtyName = false;
	if(size)
		*size = objectNameSize;
	if(objectName)
		return objectName;
	return "";
}

void Object::updateObjectName() {
	char *name;

	if(objectName)
		return;

	objectNameSize = getClassNameSize() + 9;
	name = new char[objectNameSize];

	sprintf(name, "%s%lu", getClassName(), (getObjectNum()>999999999)? 999999999 : getObjectNum());
	objectName = name;
}

static void defaultLog(const char* suffix, const char* objectName, const char* message, va_list args) {
	struct tm localtm;
	Utils::getGmTime(time(NULL), &localtm);

	fprintf(stderr, "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm.tm_year, localtm.tm_mon, localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec, suffix, objectName);
	vfprintf(stderr, message, args);
}

static Log::Level getCurrentLevel() {
	std::string level = CONFIG_GET()->log.level.get();

	if(level == "fatal" || level == "never")
		return Log::LL_Fatal;
	else if(level == "error")
		return Log::LL_Error;
	else if(level == "warning" || level == "warn")
		return Log::LL_Warning;
	else if(level == "info")
		return Log::LL_Info;
	else if(level == "debug")
		return Log::LL_Debug;
	else if(level == "trace")
		return Log::LL_Trace;
	else
		return Log::LL_Info;
}

//level is Log::Level without leading Log::LL_
#define LOG_USELOGGER(msg, level) \
	Log* logger = Log::get(); \
	va_list args; \
	va_start(args, message); \
	 \
	if(logger) \
		logger->logv(Log::LL_##level, this, message, args); \
	else if(getCurrentLevel() >= Log::LL_##level) { \
		defaultLog(#level, getObjectName(), message, args); \
	} \
	va_end(args);

void Object::trace(const char *message, ...) {
	//early check for performance boost if trace is not active
	Log* logger = Log::get();
	if(logger && logger->wouldLog(Log::LL_Trace)) {
		LOG_USELOGGER(message, Trace)
	}
}

void Object::debug(const char *message, ...) {
	LOG_USELOGGER(message, Debug)
}

void Object::info(const char *message, ...) {
	LOG_USELOGGER(message, Info)
}

void Object::warn(const char *message, ...) {
	LOG_USELOGGER(message, Warning)
}

void Object::error(const char *message, ...) {
	LOG_USELOGGER(message, Error)
}

void Object::fatal(const char *message, ...) {
	LOG_USELOGGER(message, Fatal)
}

void Object::deleteLater() {
	if(scheduledForDelete == false) {
		scheduledForDelete = true;
		EventLoop::getInstance()->addObjectToDelete(this);
	}
}
