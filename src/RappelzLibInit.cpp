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
	if(configDeclareInitCallback)
		configDeclareInitCallback();
	//Only set CONFIG_FILE_KEY
	ConfigInfo::get()->parseCommandLine(argc, argv, true);
	ConfigInfo::get()->readFile(CONFIG_GET()->app.configfile.get().c_str());
	//Set all keys given on the command line to overwrite config file values
	ConfigInfo::get()->parseCommandLine(argc, argv);
	Log::init();

	ConfigInfo::get()->dump(true);

	Log::get()->flushLog();

	return true;
}
