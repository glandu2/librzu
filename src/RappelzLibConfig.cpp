#include "RappelzLibConfig.h"

RappelzLibConfig* RappelzLibConfig::get() {
	static RappelzLibConfig instance;

	return &instance;
}

void RappelzLibConfig::Log::updateConsoleLevel(IListener *instance) {
	RappelzLibConfig::Log* thisInstance = (RappelzLibConfig::Log*)instance;

	thisInstance->consoleLevel.setDefault(thisInstance->level.get());
}
