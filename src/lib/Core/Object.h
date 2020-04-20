#ifndef OBJECT_H
#define OBJECT_H

#include <stdarg.h>
#include <stddef.h>

#include "../Extern.h"  // IWYU pragma: export

// used to count objects, "c" stand for class type, WARNING: declare public block
// getTrueClassHash: for variable used like this: Object *obj = new Actor();, obj.getTrueClassHash() return hash of
// class Actor.
// clang-format off
#define DECLARE_CLASS(C) \
	protected: struct ClassCounter { \
		unsigned long objectNo; \
		ClassCounter() : objectNo(C::__objectsCreated++) { C::__objectCount++; } \
		ClassCounter(const ClassCounter&) : objectNo(C::__objectsCreated++) { C::__objectCount++; } \
		ClassCounter& operator =(const ClassCounter&) { return *this; } \
		~ClassCounter() { C::__objectCount--; } \
	} _cc; \
	static unsigned long __objectsCreated; \
	static unsigned long __objectCount; \
	static const unsigned int __classTypeHash; \
	public: \
	virtual const char *getClassName() { return #C; } \
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
// clang-format on

class RZU_EXTERN Object {
	DECLARE_CLASS(Object)
public:
	enum Level { LL_Fatal, LL_Error, LL_Warning, LL_Info, LL_Debug, LL_Trace };

public:
	Object();
	virtual ~Object();

	void setObjectName(const char* name);
	void setObjectName(size_t maxLen, const char* format, ...) PRINTFCHECK(3, 4);
	const char* getObjectName(size_t* size = nullptr);

	void log(Level level, const char* message, ...) PRINTFCHECK(3, 4);
	void logv(Level level, const char* message, va_list args);

	static void logStatic(Level level, const char* className, const char* message, ...) PRINTFCHECK(3, 4);
	static void logStaticv(Level level, const char* className, const char* message, va_list args);

	virtual void deleteLater();
	bool isScheduledForDelete() { return scheduledForDelete; }

protected:
	void setDirtyObjectName() { dirtyName = true; }
	virtual void updateObjectName();

private:
	char* objectName;
	size_t objectNameSize;
	bool dirtyName;
	bool scheduledForDelete;
};

#endif  // OBJECT_H
