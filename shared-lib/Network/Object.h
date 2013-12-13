#ifndef OBJECT
#define OBJECT

#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include "ClassCounter.h"

//used to count objects, "c" stand for class type, WARNING: declare public block
//getTrueClassHash: for variable used like this: Object *obj = new Actor();, obj.getTrueClassHash() return hash of class Actor.
#define DECLARE_CLASS(c) protected: ClassCounter<c> _cc; \
		virtual inline unsigned long getObjectNum() { return _cc.getObjectNum(); } \
		public: virtual const char *getClassName() { return #c; } \
		virtual inline unsigned int getTrueClassHash() { return _cc.getClassHash(); } \
		static inline unsigned int getClassHash() { return ClassCounter<c>::getClassHash(); }


//generic class constructor (use of variadic templates): template<class Object, class... CtorParams> new Object(CtorParams... args);

class Object
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
