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
#define DECLARE_CLASS(C) \
	protected: struct ClassCounter { \
		unsigned long objectNo; \
		ClassCounter() : objectNo(C::__objectsCreated++) { C::__objectCount++; } \
		~ClassCounter() { C::__objectCount--; } \
	} _cc; \
	static unsigned long __objectsCreated; \
	static unsigned long __objectCount; \
	static const unsigned int __classTypeHash; \
	public: virtual const char *getClassName() { return #C; } \
	virtual unsigned int getTrueClassHash() { return __classTypeHash; } \
	static unsigned int getClassHash() { return __classTypeHash; } \
	static unsigned long getObjectCount() { return __objectCount; } \
	virtual unsigned long getObjectNum() { return _cc.objectNo; }

#define DECLARE_CLASSNAME(C, count) \
	public: virtual const char *getClassName() { return #C; } \
	virtual unsigned long getObjectNum() { return count; }


class RAPPELZLIB_EXTERN Object
{
	DECLARE_CLASS(Object)

public:
	Object(const char *name = NULL);
	virtual ~Object();

	void setObjectName(const char *name);
	void setObjectName(int maxLen, const char *format, ...);
	const char *getObjectName();
	int getObjectNameSize();

	void trace(const char *message, ...);
	void debug(const char *message, ...);
	void info(const char *message, ...);
	void warn(const char *message, ...);
	void error(const char *message, ...);
	void fatal(const char *message, ...);

	virtual void deleteLater();
private:
	char *objectName;
	int objectNameSize;
};


#endif
