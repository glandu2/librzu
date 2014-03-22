#include "Object.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "EventLoop.h"
#include "Log.h"
#include "RappelzLibConfig.h"

Object::Object(const char* name) {
    objectName = NULL;

    setObjectName(name);
}

Object::~Object() {
    if(objectName) delete[] objectName;
}

void Object::setObjectName(const char *name) {
	if(objectName)
		delete[] objectName;

    if(name) {
		objectNameSize = strlen(name)+1;
		objectName = new char[objectNameSize];
		strcpy(objectName, name);
	} else {
		objectNameSize = 0;
		objectName = NULL;
	}
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
	} else {
		objectNameSize = 0;
		objectName = NULL;
	}

	if(oldName)
		delete[] oldName;
}

const char *Object::getObjectName() {
	char *name;

	if(objectName)
		return (const char*)objectName;

	objectNameSize = strlen(getClassName()) + 6;
	name = new char[objectNameSize];

	sprintf(name, "%s%lu", getClassName(), (getObjectNum()>99999)? 99999 : getObjectNum());
	objectName = name;
	return (const char*)objectName;
}

int Object::getObjectNameSize() {
	if(objectName == nullptr)
		getObjectName(); //initialize name
	return objectNameSize;
}

static void defaultLog(const char* suffix, const char* objectName, const char* message, va_list args) {
	struct tm localtm;
	Utils::getGmTime(time(NULL), &localtm);

	fprintf(stderr, "%4d-%02d-%02d %02d:%02d:%02d %-5s %s: ", localtm.tm_year, localtm.tm_mon, localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec, suffix, objectName);
	vfprintf(stderr, message, args);
}

//level is Log::Level without leading Log::LL_
#define LOG_USELOGGER(msg, level) \
	Log* logger = Log::get(); \
	va_list args; \
	va_start(args, message); \
	 \
	if(logger) \
		logger->log(Log::LL_##level, getObjectName(), getObjectNameSize(), message, args); \
	else { \
		defaultLog(#level, getObjectName(), message, args); \
	} \
	va_end(args);

void Object::trace(const char *message, ...) {
	//early check for performance boost if trace is not active
	Log* logger = Log::get();
	if(logger && Log::LL_Trace <= logger->getMaxLevel()) {
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
	EventLoop::getInstance()->addObjectToDelete(this);
}
