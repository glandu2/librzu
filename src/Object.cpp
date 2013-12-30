#include "Object.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "EventLoop.h"
#include "Log.h"

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
		objectName = new char[strlen(name)+1];
		strcpy(objectName, name);
	} else {
		objectName = NULL;
	}
}

//maxLen without null terminator
void Object::setObjectName(int maxLen, const char *format, ...) {
	va_list args;

	if(objectName)
		delete[] objectName;

	if(format && maxLen > 0) {
		objectName = new char[maxLen+1];
		va_start(args, format);
		vsnprintf(objectName, maxLen+1, format, args);
		va_end(args);
	} else {
		objectName = NULL;
	}
}

const char *Object::getObjectName() {
	char *name;

	if(objectName) return (const char*)objectName;

	name = new char[strlen(getClassName()) + 6];
	sprintf(name, "%s%lu", getClassName(), (getObjectNum()>99999)? 99999 : getObjectNum());
	objectName = name;
	return (const char*)objectName;
}

static void defaultLog(const char* suffix, const char* objectName, const char* message, va_list args) {
	fprintf(stderr, "%s %s: ", suffix, objectName);
	vfprintf(stderr, message, args);
}

//level is Log::Level without leading Log::LL_
#define LOG_USELOGGER(msg, level) \
	Log* logger = Log::get(); \
	va_list args; \
	va_start(args, message); \
	 \
	if(logger) \
		logger->log(Log::LL_##level, getObjectName(), message, args); \
	else { \
		defaultLog(#level, getObjectName(), message, args); \
	} \
	va_end(args);

void Object::trace(const char *message, ...) {
	LOG_USELOGGER(message, Trace)
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
