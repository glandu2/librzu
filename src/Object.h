#ifndef OBJECT
#define OBJECT

#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include "RappelzLib_global.h"

//#include "ClassCounter.h"

//used to count objects, "c" stand for class type, WARNING: declare public block
//getTrueClassHash: for variable used like this: Object *obj = new Actor();, obj.getTrueClassHash() return hash of class Actor.
#define DECLARE_CLASS(c) \
	protected: struct ClassCounter { \
		static unsigned long objectsCreated; \
		static unsigned long objectCount; \
		static const unsigned int classTypeHash; \
		unsigned long objectNo; \
		ClassCounter() : objectNo(objectsCreated++) { objectCount++; } \
		~ClassCounter() { objectCount--; } \
	} _cc; \
	public: virtual const char *getClassName() { return #c; } \
	virtual unsigned int getTrueClassHash() { return _cc.classTypeHash; } \
	static unsigned int getClassHash() { return ClassCounter::classTypeHash; } \
	static unsigned long getObjectCount() { return ClassCounter::objectCount; } \
	virtual unsigned long getObjectNum() { return _cc.objectNo; }



//generic class constructor (use of variadic templates): template<class Object, class... CtorParams> new Object(CtorParams... args);

class RAPPELZLIB_EXTERN Object
{
	DECLARE_CLASS(Object)

	public:
		Object(const char *name = NULL);
		virtual ~Object();

		void setObjectName(const char *name);
		const char *getObjectName();

		void log(const char *message, ...);
		void warn(const char *message, ...);
		void error(const char *message, ...);

		virtual void deleteLater();
    private:
		char *objectName;
};


#endif
