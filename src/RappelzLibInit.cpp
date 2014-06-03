#include "RappelzLibInit.h"
#include "RappelzLibConfig.h"
#include "Log.h"

#ifdef __unix
#include <signal.h>
#endif

void disableSigPipe() {
#ifdef __unix
	signal(SIGPIPE, SIG_IGN);
#endif
}

bool RappelzLibInit(int argc, char **argv, ClientConfigInit configDeclareInitCallback) {
	disableSigPipe();

	RappelzLibConfig::init();


	return true;
}
