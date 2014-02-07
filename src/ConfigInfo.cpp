#include "ConfigInfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RappelzLibConfig.h"
#include <map>

bool ConfigValue::check(Type expectedType, bool soft) {
	if(expectedType != type && type != None) {
		const char* typeStr[] = {
			"Bool",
			"Integer",
			"Float",
			"String",
			"None"
		};
		fprintf(stderr, "%s: config value type mismatch for %s[%s], tried to get as %s. Check the config file.\n",
				soft ? "Warning" : "Fatal",
				keyName != nullptr ? keyName->c_str() : "unknown key",
				typeStr[type],
				typeStr[expectedType]);
		if(!soft)
			abort();
		return false;
	}
	return true;
}


// ConfigInfo ////////////////////////////////////////////////////////////////////////

ConfigInfo::~ConfigInfo() {
	for(auto it = config.begin(); it != config.end(); ++it) {
		delete it->second;
	}
}

ConfigInfo* ConfigInfo::get() {
	static ConfigInfo instance;

	return &instance;
}

ConfigValue* ConfigInfo::getValue(const std::string& key, bool createIfNonExistant) {
	std::unordered_map<std::string, ConfigValue*>::const_iterator it;

	it = config.find(key);
	if(it != config.cend()) {
		return it->second;
	} else {
		if(createIfNonExistant) {
			ConfigValue* v = new ConfigValue(ConfigValue::None);
			addValue(key, v);
			return v;
		} else {
			return nullptr;
		}
	}
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
		ConfigValue* v = getValue(keyStr, false);
		if(v == nullptr) {
			warn("Unknown key \"%s\", ignoring\n", keyStr.c_str());
			continue;
		}

		switch(v->getType()) {
			case ConfigValue::Bool:
				v->set(!strcmp(value, "true") || !strcmp(value, "1"));
				break;

			case ConfigValue::Integer:
				v->set(atoi(value));
				break;

			case ConfigValue::Float:
				v->set(atof(value));
				break;

			case ConfigValue::String:
				v->set(std::string(value));
				break;

			case ConfigValue::None:
				error("Key \"%s\" data type is None, ignoring\n", keyStr.c_str());
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

		ConfigValue* v = getValue(keyStr, false);
		if(v == nullptr) {
			warn("Unknown key \"%s\" in config file, ignoring\n", keyStr.c_str());
			continue;
		}

		switch(v->getType()) {
			case ConfigValue::Bool:
				v->set(!strcmp(value, "true") || !strcmp(value, "1"));
				break;

			case ConfigValue::Integer:
				v->set(atoi(value));
				break;

			case ConfigValue::Float:
				v->set(atof(value));
				break;

			case ConfigValue::String:
				v->set(std::string(value));
				break;

			case ConfigValue::None:
				error("Key \"%s\" data type is None, ignoring\n", keyStr.c_str());
				break;
		}
	}

	fclose(file);

	return true;
}

bool ConfigInfo::writeFile(const char *filename) {
	FILE* file;

	file = fopen(filename, "wb");
	if(!file)
		return false;
	dump(file, false);
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
void ConfigInfo::dump(FILE *out, bool showDefault) {
	std::map<std::string, ConfigValue*>::const_iterator it, itEnd;
	std::string val;
	bool isDefault = true;

	std::map<std::string, ConfigValue*> ordered(config.cbegin(), config.cend());

	for(it = ordered.cbegin(), itEnd = ordered.cend(); it != itEnd; ++it) {
		ConfigValue* v = it->second;
		char type = 'd';
		switch(v->getType()) {
			case ConfigValue::Bool:
				type = 'B';
				val = v->get(false) ? "true" : "false";
				isDefault = v->get(false).isDefault();
				break;

			case ConfigValue::Integer:
				type = 'N';
				val = INT2STR(v->get(0));
				isDefault = v->get(0).isDefault();
				break;

			case ConfigValue::Float:
				type = 'F';
				val = FLOAT2STR(v->get(0.0f));
				isDefault = v->get(0.0f).isDefault();
				break;

			case ConfigValue::String:
				type = 'S';
				val = v->get("<NULL>");
				isDefault = v->get("<NULL>").isDefault();
				break;

			case ConfigValue::None:
				type = '0';
				val = "<NONE>";
				isDefault = true;
				break;
		}
		if(!isDefault || showDefault)
			fprintf(out, "%c%c%s:%s\n", type, isDefault ? '*' : ' ', it->first.c_str(), val.c_str());
	}
}
