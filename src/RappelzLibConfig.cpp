#include "RappelzLibConfig.h"
#include "RappelzLibGitVersion.h"

RappelzLibConfig* RappelzLibConfig::get() {
	static RappelzLibConfig instance;

	return &instance;
}

void RappelzLibConfig::init() {
	RappelzLibConfig::get();
	CFG("core.version", RappelzLibVersion);
}

void RappelzLibConfig::Log::updateConsoleLevel(IListener *instance) {
	RappelzLibConfig::Log* thisInstance = (RappelzLibConfig::Log*)instance;

	thisInstance->consoleLevel.setDefault(thisInstance->level.get());
}
