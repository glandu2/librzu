#include "StartableObject.h"
#include <string.h>

StartableObject::StartableObject() : name(nullptr) {}

StartableObject::~StartableObject() {
	delete[] name;
}

void StartableObject::setName(const char* name) {
	delete[] this->name;
	if(name) {
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
	}
}
