#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include "RappelzLib_global.h"

class RAPPELZLIB_EXTERN CrashHandler
{
public:
	// Sets exception handlers that work on per-process basis
	static void setProcessExceptionHandlers();

	// Installs C++ exception handlers that function on per-thread basis
	static void setThreadExceptionHandlers();

	static void setDumpMode(int _dumpMode);
};

#endif // CRASHHANDLER_H
