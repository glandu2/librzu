#ifndef CLASSCOUNTER_H
#define CLASSCOUNTER_H

#include "Extern.h"
#include <map>
#include <string>

// Provide a counter for each classes so each instance has an unique name based on it's class name

// Contain class count as we instanciate them. Class which are not instanciated are not counted.
// This provide a way to convert a class type to an integer hash.
// this var is implemented in ClassCounter.cpp
extern unsigned int RZU_EXTERN ClassCounter__internal_object_engine_class_counter;

std::map<std::string, unsigned long*> RZU_EXTERN& getObjectsCount();

#define DECLARE_CLASSCOUNT_STATIC(classname) \
	unsigned long classname::__objectCount = 0; \
	unsigned long classname::__objectsCreated = 0; \
	const unsigned int classname::__classTypeHash = \
	    (getObjectsCount().insert(std::pair<std::string, unsigned long*>(#classname, &classname::__objectCount)), \
	     ClassCounter__internal_object_engine_class_counter++);

#endif  // CLASSCOUNTER_H
