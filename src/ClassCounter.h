#ifndef CLASSCOUNTER_H
#define CLASSCOUNTER_H

#include "RappelzLib_global.h"

//Provide a counter for each classes so each instance has an unique name based on it's class name

//Contain class count as we instanciate them. Class which are not instanciated are not counted.
//This provide a way to convert a class type to an integer hash.
//this var is implemented in ClassCounter.cpp
extern unsigned int RAPPELZLIB_EXTERN ClassCounter__internal_object_engine_class_counter;

template<class T>
class RAPPELZLIB_EXTERN ClassCounter
{
	public:
		ClassCounter() { objectNo = objectCount++; }
		ClassCounter(const ClassCounter &) { objectNo = objectCount++; }
		~ClassCounter() { objectCount--; }
		inline unsigned long getObjectNum() { return objectNo; }
		static inline unsigned int getClassHash() { return classTypeHash; }
		static inline unsigned long getObjectCount() { return objectCount; }
	protected:
		static unsigned long objectCount;
		static const unsigned int classTypeHash;
		unsigned long objectNo;
	private:
};
/*
template<class T>
unsigned long ClassCounter<T>::objectCount = 0;

template<class T>
const unsigned int ClassCounter<T>::classTypeHash = ClassCounter__internal_object_engine_class_counter++;
*/
#endif // CLASSCOUNTER_H
