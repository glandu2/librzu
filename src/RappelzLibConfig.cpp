#include "RappelzLibConfig.h"
#include "rzuGitVersion.h"

GlobalCoreConfig* GlobalCoreConfig::get() {
	static GlobalCoreConfig instance;

	return &instance;
}

void GlobalCoreConfig::init() {
	GlobalCoreConfig::get();
	CFG_CREATE("core.version", rzuVersion);
}

void GlobalCoreConfig::Log::updateConsoleLevel(IListener *instance) {
	GlobalCoreConfig::Log* thisInstance = (GlobalCoreConfig::Log*)instance;

	thisInstance->consoleLevel.setDefault(thisInstance->level.get());
}
