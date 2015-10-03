#include "DbQueryJob.h"

DbBindingLoader* DbBindingLoader::get() {
    static DbBindingLoader dbBindingLoader;
    return &dbBindingLoader;
}

void DbBindingLoader::addBinding(DbBindingLoader::InitBindingFunction initFunction) {
    dbQueryBindingInits.push_back(initFunction);
}

void DbBindingLoader::initAll(DbConnectionPool* connectionPool) {
    auto it = dbQueryBindingInits.begin();
    for(; it != dbQueryBindingInits.end(); ++it) {
        InitBindingFunction initFunction = (*it);
        (*initFunction)(connectionPool);
    }

    dbQueryBindingInits.swap(std::vector<InitBindingFunction>());
}
