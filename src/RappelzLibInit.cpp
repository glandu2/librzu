#include "RappelzLibInit.h"
#include "RappelzLibConfig.h"
#include "Log.h"
#include "CrashHandler.h"

#ifdef __unix
#include <signal.h>
#endif

void disableSigPipe() {
#ifdef __unix
	signal(SIGPIPE, SIG_IGN);
#endif
}

bool RappelzLibInit() {
	disableSigPipe();
	CrashHandler::setProcessExceptionHandlers();
	CrashHandler::setThreadExceptionHandlers();

	return true;
}
