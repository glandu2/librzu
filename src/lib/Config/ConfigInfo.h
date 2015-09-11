#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include "Core/Object.h"
#include <unordered_map>
#include <string>
#include <vector>

#include "ConfigParamVal.h"
#include "ConfigStatVal.h"

class IWritableConsole;

class RZU_EXTERN ConfigInfo : public Object
{
	DECLARE_CLASS(ConfigInfo)

public:
	void init(int argc, char **argv);

	void parseCommandLine(int argc, char **argv, bool onlyConfigFileLocation = false);
	bool readFile(const char *filename, int fileDepth = 0);
	void dump(bool showDefault = true);

	ConfigValue* getValue(const std::string& key);

	template<template<class> class U, typename T>
	U<T>& createValue(const char* key, T def, bool isHidden = false) {
		U<T>* value = new U<T>(def);
		value->setHidden(isHidden);

		if(!addValue(std::string(key), value).second) {
			log(LL_Error, "Config value already exist: %s\n", key);
		}

		return *value;
	}

	template<template<class> class U>
	U<std::string>& createValue(const char* key, const char* def, bool isHidden = false) {
		return createValue<U, std::string>(key, std::string(def), isHidden);
	}

	template<template<class> class U, typename T>
	U<T>& createValue(const std::string& key, T def, bool isHidden = false) {
		return createValue<U, T>(key.c_str(), def, isHidden);
	}

	template<template<class> class U>
	U<std::string>& createValue(const std::string& key, const char* def, bool isHidden = false) {
		return createValue<U>(key.c_str(), def, isHidden);
	}

	static ConfigInfo* get();

protected:
	static void commandSetEnv(IWritableConsole* console, const std::vector<std::string>& args);
	static void commandGetEnv(IWritableConsole* console, const std::vector<std::string>& args);

private:
	ConfigInfo() {}
	~ConfigInfo();

	static bool parseLine(char* line, std::string &key, std::string &value);

	std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> addValue(const std::string& key, ConfigValue* v);
	std::unordered_map<std::string, ConfigValue*> config;
};


#define CFG_CREATE(...) ConfigInfo::get()->createValue<cval>(__VA_ARGS__)
#define CFG_STAT_CREATE(...) ConfigInfo::get()->createValue<cstatval>(__VA_ARGS__)
#define CFG_GET(key) ConfigInfo::get()->getValue(key)

#endif // CONFIGINFO_H
