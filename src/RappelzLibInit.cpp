#include "RappelzLibInit.h"
#include "private/RappelzLibConfig.h"
#include "Log.h"

bool RappelzLibInit(int argc, char **argv, ClientConfigInit configDeclareInitCallback) {
	RappelzLibConfig::get(); //get will init
	if(configDeclareInitCallback)
		configDeclareInitCallback();
	ConfigInfo::get()->parseCommandLine(argc, argv);
	ConfigInfo::get()->readFile(CONFIG_GET()->app.configfile.get().c_str());
	Log::init();

	return true;
}
