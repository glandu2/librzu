#include "ConfigInfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RappelzLibConfig.h"
#include <map>

ConfigInfo::~ConfigInfo() {
	for(auto it = config.begin(); it != config.end(); ++it) {
		delete it->second;
	}
}

void ConfigInfo::init(int argc, char **argv) {
	RappelzLibConfig::init();

	parseCommandLine(argc, argv, true);
	readFile(RappelzLibConfig::get()->app.configfile.get().c_str());
	//Set all keys given on the command line to overwrite config file values
	parseCommandLine(argc, argv);
}

ConfigInfo* ConfigInfo::get() {
	static ConfigInfo instance;

	return &instance;
}

ConfigValue* ConfigInfo::getValue(const std::string& key) {
	std::unordered_map<std::string, ConfigValue*>::const_iterator it;

	it = config.find(key);
	if(it != config.cend())
		return it->second;
	else
		return nullptr;
}

std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> ConfigInfo::addValue(const std::string& key, ConfigValue* v) {
	std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> it = config.insert(std::pair<std::string, ConfigValue*>(key, v));
	if(it.second)
		v->setKeyName(&it.first->first);
	return it;
}

void ConfigInfo::parseCommandLine(int argc, char **argv, bool onlyConfigFileLocation) {
	int i;
	char* key;
	char* value;

	for(i = 1; i < argc; i++) {
		if(argv[i][0] != '/' && argv[i][0] != '-')
			continue;

		key = argv[i];
		while(!isalpha(*key) && *key != '\0')
			key++;

		if(*key == '\0')
			continue;

		value = strpbrk(key, ":=");
		if(!value)
			continue;

		std::string keyStr(key, value - key);

		//Check only config file location value if requested
		if(onlyConfigFileLocation && keyStr != CONFIG_FILE_KEY)
			continue;

		value++;
		ConfigValue* v = getValue(keyStr);
		if(v == nullptr) {
			warn("Unknown key \"%s\", ignoring\n", keyStr.c_str());
			continue;
		}

		switch(v->getType()) {
			case ConfigValue::Bool:
				v->setBool(!strcmp(value, "true") || !strcmp(value, "1"));
				break;

			case ConfigValue::Integer:
				v->setInt(atoi(value));
				break;

			case ConfigValue::Float:
				v->setFloat((float)atof(value));
				break;

			case ConfigValue::String:
				v->setString(value);
				break;
		}
	}
}

bool ConfigInfo::readFile(const char* filename) {
	FILE* file;
	char line[1024];
	char *p, *key, *value;

	file = fopen(filename, "rb");
	if(!file) {
		info("No config file (%s)\n", filename);
		return false;
	}

	info("Using config file %s\n", filename);

	while(fgets(line, 1024, file)) {
		size_t len = strlen(line);

		//remove leading spaces
		p = line;
		while(isspace(*p) && *p)
			p++;

		key = p;
		if(key[0] == '#' || key[0] == '\0')	//a comment or end of line (nothing on the line)
			continue;

		//remove trailing spaces
		p = line + len - 1;
		while(isspace(*p) && p > key)
			p--;
		if(p == key)
			continue;
		*(p+1) = 0;

		p = strpbrk(key, ":=");
		if(!p)
			continue;
		*p = 0;
		value = p+1;

		std::string keyStr(key);

		ConfigValue* v = getValue(keyStr);
		if(v == nullptr) {
			warn("Unknown key \"%s\" in config file, ignoring\n", keyStr.c_str());
			continue;
		}

		switch(v->getType()) {
			case ConfigValue::Bool:
				v->setBool(!strcmp(value, "true") || !strcmp(value, "1"));
				break;

			case ConfigValue::Integer:
				v->setInt(atoi(value));
				break;

			case ConfigValue::Float:
				v->setFloat((float)atof(value));
				break;

			case ConfigValue::String:
				v->setString(value);
				break;
		}
	}

	fclose(file);

	return true;
}

#ifdef _MSC_VER
#define INT2STR(i) std::to_string((long long)(i))
#define FLOAT2STR(i) std::to_string((long double)(i))
#else
#define INT2STR(i) std::to_string(i)
#define FLOAT2STR(i) std::to_string(i)
#endif
void ConfigInfo::dump(bool showDefault) {
	std::map<std::string, ConfigValue*>::const_iterator it, itEnd;
	std::string val;

	info("Config dump:\n");

	std::map<std::string, ConfigValue*> ordered(config.cbegin(), config.cend());

	for(it = ordered.cbegin(), itEnd = ordered.cend(); it != itEnd; ++it) {
		ConfigValue* v = it->second;
		char type = 'd';
		switch(v->getType()) {
			case ConfigValue::Bool:
				type = 'B';
				val = v->getBool() ? "true" : "false";
				break;

			case ConfigValue::Integer:
				type = 'N';
				val = INT2STR(v->getInt());
				break;

			case ConfigValue::Float:
				type = 'F';
				val = FLOAT2STR(v->getFloat());
				break;

			case ConfigValue::String:
				type = 'S';
				val = v->getString();
				break;
		}
		if(!v->isDefault() || showDefault)
			info("%c%c%s:%s\n", type, v->isDefault() ? '*' : ' ', it->first.c_str(), val.c_str());
	}
}
