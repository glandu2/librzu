#include "DbBindingLoader.h"

DbBindingLoader* DbBindingLoader::get() {
	static DbBindingLoader dbBindingLoader;
	return &dbBindingLoader;
}

void DbBindingLoader::addInitBinding(InitBindingFunction initFunction) {
	dbQueryBindingInits.push_back(initFunction);
}

void DbBindingLoader::addDeinitBinding(DeinitBindingFunction deinitFunction) {
	dbQueryBindingDeinits.push_back(deinitFunction);
}

void DbBindingLoader::initAll(DbConnectionPool* connectionPool) {
	auto it = dbQueryBindingInits.begin();
	for(; it != dbQueryBindingInits.end(); ++it) {
		InitBindingFunction initFunction = (*it);
		(*initFunction)(connectionPool);
	}

	std::vector<InitBindingFunction> empty;
	dbQueryBindingInits.swap(empty);
}

void DbBindingLoader::deinitAll() {
	auto it = dbQueryBindingDeinits.begin();
	for(; it != dbQueryBindingDeinits.end(); ++it) {
		DeinitBindingFunction deinitFunction = (*it);
		(*deinitFunction)();
	}

	std::vector<DeinitBindingFunction> empty;
	dbQueryBindingDeinits.swap(empty);
}
