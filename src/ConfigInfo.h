#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include "Object.h"
#include <unordered_map>
#include <string>

#include "ConfigParamVal.h"
#include "ConfigStatVal.h"

class RAPPELZLIB_EXTERN ConfigInfo : public Object
{
	DECLARE_CLASS(ConfigInfo)

public:
	void parseCommandLine(int argc, char **argv, bool onlyConfigFileLocation = false);
	bool readFile(const char *filename);
	void dump(bool showDefault);

	ConfigValue* getValue(const std::string& key);

	/*template<typename T> cval<T>& createParam(const char* key, T def);
	template<typename T> cval<T>& createParam(const std::string& key, T def);
	cval<std::string>& createParam(const char* key, const char* def);
	cval<std::string>& createParam(const std::string& key, const char* def);*/

	//Create parameter value
	/*template<typename T> cstatval<T>& createStat(const char* key, T def);
	template<typename T> cstatval<T>& createStat(const std::string& key, T def);
	cstatval<std::string>& createStat(const char* key, const char* def);
	cstatval<std::string>& createStat(const std::string& key, const char* def);*/

	template<template<class> class U, typename T>
	U<T>& createValue(const char* key, T def) {
		U<T>* value = new U<T>(def);

		if(!addValue(std::string(key), value).second) {
			error("Config value already exist: %s\n", key);
		}

		return *value;
	}

	template<template<class> class U>
	U<std::string>& createValue(const char* key, const char* def) {
		U<std::string>* value = new U<std::string>(std::string(def));

		if(!addValue(std::string(key), value).second) {
			error("Config value already exist: %s\n", key);
		}

		return *value;
	}

	template<template<class> class U, typename T>
	U<T>& createValue(const std::string& key, T def) {
		return createValue<U, T>(key.c_str(), def);
	}

	template<template<class> class U>
	U<std::string>& createValue(const std::string& key, const char* def) {
		return createValue<U>(key.c_str(), def);
	}

	static ConfigInfo* get();

private:
	ConfigInfo() {}
	~ConfigInfo();

	std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> addValue(const std::string& key, ConfigValue* v);
	std::unordered_map<std::string, ConfigValue*> config;
};


#define CFG_CREATE(key, defaultValue) ConfigInfo::get()->createValue<cval>(key, defaultValue)
#define CFG_STAT_CREATE(key, defaultValue) ConfigInfo::get()->createValue<cstatval>(key, defaultValue)
#define CFG_GET(key) ConfigInfo::get()->getValue(key)


////////////////////////////////
///Create parameter value
////////////////////////////////
/*template<typename T>
cval<T>& createParam(const char* key, T def) {
	cval<T>* value = new cval<T>(def);

	if(!addValue(std::string(key), value).second) {
		error("Config value already exist: %s\n", key);
	}

	return *value;
}

cval<std::string>& createParam(const char* key, const char* def) {
	cval<std::string>* value = new cval<std::string>(std::string(def));

	if(!addValue(std::string(key), value).second) {
		error("Config value already exist: %s\n", key);
	}

	return *value;
}

template<typename T>
cval<T>& createParam(const std::string& key, T def) {
	return createParam<T>(key.c_str(), def);
}

cval<std::string>& createParam(const std::string& key, const char* def) {
	return createParam(key.c_str(), def);
}*/


////////////////////////////////
///Create stat value
////////////////////////////////
/*template<typename T>
cstatval<T>& createStat(const char* key, T def) {
	cstatval<T>* value = new cstatval<T>(def);

	if(!addValue(std::string(key), value).second) {
		error("Config value already exist: %s\n", key);
	}

	return *value;
}

cstatval<std::string>& createStat(const char* key, const char* def) {
	cstatval<std::string>* value = new cstatval<std::string>(std::string(def));

	if(!addValue(std::string(key), value).second) {
		error("Config value already exist: %s\n", key);
	}

	return *value;
}

template<typename T>
cstatval<T>& createStat(const std::string& key, T def) {
	return createParam<T>(key.c_str(), def);
}

cstatval<std::string>& createStat(const std::string& key, const char* def) {
	return createParam(key.c_str(), def);
}*/

#endif // CONFIGINFO_H
