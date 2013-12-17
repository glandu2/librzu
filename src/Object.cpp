#include "Object.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "EventLoop.h"

Object::Object(const char* name) {
    objectName = NULL;

    setObjectName(name);
}

Object::~Object() {
    if(objectName) delete[] objectName;
}

void Object::setObjectName(const char *name) {
    if(objectName) delete[] objectName;
    if(name) {
		objectName = new char[strlen(name)+1];
		strcpy(objectName, name);
    } else objectName = NULL;
}

const char *Object::getObjectName() {
	char *name;

	if(objectName) return (const char*)objectName;

	name = new char[strlen(getClassName()) + 6];
	sprintf(name, "%s%lu", getClassName(), (getObjectNum()>99999)? 99999 : getObjectNum());
	objectName = name;
	return (const char*)objectName;
}

void Object::log(const char *message, ...) {
	printf("%s: ", getObjectName());
	va_list vl;
	va_start(vl,message);
	vprintf(message, vl);
	va_end(vl);
}
void Object::warn(const char *message, ...) {
	printf("Warning ! %s: ", getObjectName());
	va_list vl;
	va_start(vl,message);
	vprintf(message, vl);
	va_end(vl);
}
void Object::error(const char *message, ...) {
	printf("Error ! %s: ", getObjectName());
	va_list vl;
	va_start(vl,message);
	vprintf(message, vl);
	va_end(vl);
}

void Object::deleteLater() {
	EventLoop::getInstance()->addObjectToDelete(this);
}
