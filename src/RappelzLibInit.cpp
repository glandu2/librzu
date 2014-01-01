#include "RappelzLibInit.h"
#include "RappelzLibConfig.h"
#include "Log.h"

bool RappelzLibInit(int argc, char **argv, ClientConfigInit configDeclareInitCallback) {
	RappelzLibConfig::get(); //get will init
	if(configDeclareInitCallback)
		configDeclareInitCallback();
	//Only set CONFIG_FILE_KEY
	ConfigInfo::get()->parseCommandLine(argc, argv, true);
	ConfigInfo::get()->readFile(CONFIG_GET()->app.configfile.get().c_str());
	//Set all keys given on the command line to overwrite config file values
	ConfigInfo::get()->parseCommandLine(argc, argv);
	ConfigInfo::get()->dump(stderr, true);
	Log::init();

	return true;
}
