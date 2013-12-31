#include "ConfigInfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void ConfigInfo::parseCommandLine(int argc, char **argv) {
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
		value++;
		ConfigValue* v = getValue(keyStr, false);
		if(v == nullptr) {
			debug("Unknown key %s, ignoring\n", keyStr.c_str());
			continue;
		}
		if(v->getType() == ConfigValue::None) {
			debug("Key %s data type is None, ignoring\n", keyStr.c_str());
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
				debug("Key %s data type is None, ignoring\n", keyStr.c_str());
				break;
		}
	}
}

bool ConfigInfo::readFile(const char* filename) {
	FILE* file;
	char line[1024];
	char* p;
	ConfigValue* v;
	typedef std::unordered_map<std::string, ConfigValue*>::iterator Iterator;

	file = fopen(filename, "rb");
	if(!file) {
		info("No config file (%s)\n", filename);
		return false;
	}

	while(fgets(line, 1024, file)) {
		size_t len = strlen(line);
		if(len < 3)   //minimum: type + space + key char
			continue;

		p = line + len - 1;
		while(isspace(*p))
			p--;
		*(p+1) = 0;

		p = strchr(line, ':');
		if(!p)
			continue;
		*p = 0;
		p++;

		switch(line[0]) {
			case 'B':
				v = new ConfigValue(ConfigValue::Bool);
				v->set(!strcmp(p, "true") || !strcmp(p, "1"));
				break;

			case 'N':
			case 'I':
				v = new ConfigValue(ConfigValue::Integer);
				v->set(atoi(p));
				break;

			case 'F':
				v = new ConfigValue(ConfigValue::Float);
				v->set(atof(p));
				break;

			case 'S':
				v = new ConfigValue(ConfigValue::String);
				v->set(std::string(p));
				break;

			default:
				continue;
		}
		std::pair<Iterator, bool> it = addValue(std::string(line+2), v);
		if(it.second == false) {
			ConfigValue* orig = it.first->second;
			orig->set(v);
		}

		v = nullptr;
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
	std::unordered_map<std::string, ConfigValue*>::const_iterator it, itEnd;
	std::string val;
	bool isDefault;

	for(it = config.cbegin(), itEnd = config.cend(); it != itEnd; ++it) {
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
