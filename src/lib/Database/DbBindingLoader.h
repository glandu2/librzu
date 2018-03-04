#ifndef DBBINDINGLOADER_H
#define DBBINDINGLOADER_H

#include "../Extern.h"
#include <vector>

class DbConnectionPool;

class RZU_EXTERN DbBindingLoader {
public:
	typedef void (*InitBindingFunction)(DbConnectionPool*);
	typedef void (*DeinitBindingFunction)();

	static DbBindingLoader* get();
	void addInitBinding(InitBindingFunction initFunction);
	void addDeinitBinding(DeinitBindingFunction deinitFunction);
	void initAll(DbConnectionPool* connectionPool);
	void deinitAll();

private:
	DbBindingLoader() {}
	DbBindingLoader(const DbBindingLoader&) {}
	std::vector<InitBindingFunction> dbQueryBindingInits;
	std::vector<DeinitBindingFunction> dbQueryBindingDeinits;
};

#endif  // DBBINDINGLOADER_H
