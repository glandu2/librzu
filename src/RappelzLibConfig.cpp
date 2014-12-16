#include "RappelzLibConfig.h"
#include "rzuGitVersion.h"

RappelzLibConfig* RappelzLibConfig::get() {
	static RappelzLibConfig instance;

	return &instance;
}

void RappelzLibConfig::init() {
	RappelzLibConfig::get();
	CFG_CREATE("core.version", rzuVersion);
}

void RappelzLibConfig::Log::updateConsoleLevel(IListener *instance) {
	RappelzLibConfig::Log* thisInstance = (RappelzLibConfig::Log*)instance;

	thisInstance->consoleLevel.setDefault(thisInstance->level.get());
}
