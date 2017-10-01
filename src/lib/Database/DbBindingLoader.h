#ifndef DBBINDINGLOADER_H
#define DBBINDINGLOADER_H

#include "../Extern.h"
#include <vector>

class DbConnectionPool;

class RZU_EXTERN DbBindingLoader {
public:
	typedef void (*InitBindingFunction)(DbConnectionPool*);

	static DbBindingLoader* get();
	void addBinding(InitBindingFunction initFunction);
	void initAll(DbConnectionPool* connectionPool);

private:
	DbBindingLoader() {}
	DbBindingLoader(const DbBindingLoader&) {}
	std::vector<InitBindingFunction> dbQueryBindingInits;
};

#endif  // DBBINDINGLOADER_H
