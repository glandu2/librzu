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
	void init(int argc, char **argv);

	void parseCommandLine(int argc, char **argv, bool onlyConfigFileLocation = false);
	bool readFile(const char *filename);
	void dump(bool showDefault = true);

	ConfigValue* getValue(const std::string& key);

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

#endif // CONFIGINFO_H
