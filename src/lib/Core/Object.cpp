#include "Object.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "EventLoop.h"
#include "Log.h"
#include "Utils.h"
#include "Config/GlobalCoreConfig.h"

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
void Object::setObjectName(size_t maxLen, const char *format, ...) {
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

	fprintf(stdout, "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm.tm_year, localtm.tm_mon, localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec, suffix, objectName);
	vfprintf(stdout, message, args);
}

static Object::Level getCurrentLevel() {
	std::string level = CONFIG_GET()->log.level.get();

	if(level == "fatal" || level == "never")
		return Object::LL_Fatal;
	else if(level == "error")
		return Object::LL_Error;
	else if(level == "warning" || level == "warn")
		return Object::LL_Warning;
	else if(level == "info")
		return Object::LL_Info;
	else if(level == "debug")
		return Object::LL_Debug;
	else if(level == "trace")
		return Object::LL_Trace;
	else
		return Object::LL_Info;
}

void Object::logv(Level level, const char* message, va_list args) {
	Log* logger = Log::get();

	if(logger)
		logger->logv(level, this, message, args);
	else if(getCurrentLevel() >= level) {
		const char* levelStr = "Unknown";
		switch(level) {
			case LL_Fatal: levelStr = "Fatal"; break;
			case LL_Error: levelStr = "Error"; break;
			case LL_Warning: levelStr = "Warning"; break;
			case LL_Info: levelStr = "Info"; break;
			case LL_Debug: levelStr = "Debug"; break;
			case LL_Trace: levelStr = "Trace"; break;
		}

		defaultLog(levelStr, getObjectName(), message, args);
	}
}

void Object::log(Level level, const char *message, ...) {
	//early check for performance boost if trace is not active
	Log* logger = Log::get();
	if(level < LL_Trace || logger && logger->wouldLog(level)) {
		va_list args;
		va_start(args, message);
		logv(LL_Trace, message, args);
		va_end(args);
	}
}

void Object::logStaticv(Level level, const char* className, const char* message, va_list args) {
	Log* logger = Log::get();

	if(logger)
		logger->logv(level, className, strlen(className), message, args);
	else if(getCurrentLevel() >= level) {
		const char* levelStr = "Unknown";
		switch(level) {
			case LL_Fatal: levelStr = "Fatal"; break;
			case LL_Error: levelStr = "Error"; break;
			case LL_Warning: levelStr = "Warning"; break;
			case LL_Info: levelStr = "Info"; break;
			case LL_Debug: levelStr = "Debug"; break;
			case LL_Trace: levelStr = "Trace"; break;
		}

		defaultLog(levelStr, className, message, args);
	}
}

void Object::logStatic(Level level, const char* className, const char *message, ...) {
	Log* logger = Log::get();
	if(level < LL_Trace || logger && logger->wouldLog(level)) {
		va_list args;
		va_start(args, message);
		logStaticv(level, className, message, args);
		va_end(args);
	}
}

void Object::deleteLater() {
	if(scheduledForDelete == false) {
		scheduledForDelete = true;
		EventLoop::getInstance()->addObjectToDelete(this);
	}
}
