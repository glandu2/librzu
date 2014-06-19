#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include "RappelzLib_global.h"
#include "uv.h"

class RAPPELZLIB_EXTERN CrashHandler
{
public:
	typedef void (*TerminateCallback)(void* instance);
public:
	// Sets exception handlers that work on per-process basis
	static void setProcessExceptionHandlers();

	// Installs C++ exception handlers that function on per-thread basis
	static void setThreadExceptionHandlers();

	static void setDumpMode(int _dumpMode);

	static void setTerminateCallback(TerminateCallback callback, void* instance);

	static void terminate() { uv_async_send(&asyncCallback); }

private:
	static void onTerminate(uv_async_t*);
	static uv_async_t asyncCallback;
	static void* callbackInstance;
};

#endif // CRASHHANDLER_H
