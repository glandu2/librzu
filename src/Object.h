#ifndef OBJECT
#define OBJECT

#include <string>

#include "RappelzLib_global.h"

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
	static const char* getStaticClassName() { return #C; } \
	virtual unsigned int getClassNameSize() { return sizeof(#C); } \
	static unsigned int getStaticClassNameSize() { return sizeof(#C); } \
	virtual unsigned int getTrueClassHash() { return __classTypeHash; } \
	static unsigned int getClassHash() { return __classTypeHash; } \
	static unsigned long getObjectCount() { return __objectCount; } \
	virtual unsigned long getObjectNum() { return _cc.objectNo; }

#define DECLARE_CLASSNAME(C, count) \
	public: virtual const char *getClassName() { return #C; } \
	virtual unsigned int getClassNameSize() { return sizeof(#C); } \
	virtual unsigned long getObjectNum() { return count; }


class RAPPELZLIB_EXTERN Object
{
	DECLARE_CLASS(Object)

public:
	Object();
	virtual ~Object();

	void setObjectName(const char *name);
	void setObjectName(int maxLen, const char *format, ...) PRINTFCHECK(3, 4);
	const char *getObjectName(size_t *size = nullptr);

	void trace(const char *message, ...) PRINTFCHECK(2, 3);
	void debug(const char *message, ...) PRINTFCHECK(2, 3);
	void info(const char *message, ...) PRINTFCHECK(2, 3);
	void warn(const char *message, ...) PRINTFCHECK(2, 3);
	void error(const char *message, ...) PRINTFCHECK(2, 3);
	void fatal(const char *message, ...) PRINTFCHECK(2, 3);

	virtual void deleteLater();

protected:
	void setDirtyObjectName() { dirtyName = true; }
	virtual void updateObjectName();

private:
	char *objectName;
	size_t objectNameSize;
	bool dirtyName;
};


#endif
