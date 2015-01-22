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
	GlobalCoreConfig::init();

	parseCommandLine(argc, argv, true);
	readFile(GlobalCoreConfig::get()->app.configfile.get().c_str());
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

bool ConfigInfo::readFile(const char* filename, int fileDepth) {
	FILE* file;
	char line[1024];
	char *p, *key, *value;
	int lineNumber = 0;

	if(fileDepth > 10) {
		error("Too many file inclusions (%d) while reading %s, continuing without going deeper\n", fileDepth, filename);
		return false;
	}

	file = fopen(filename, "rb");
	if(!file) {
		info("No config file (%s)\n", filename);
		return false;
	}

	info("Reading config file %s\n", filename);

	while(fgets(line, 1024, file)) {
		size_t len = strlen(line);
		lineNumber++;

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

		trace("%s:%d: Read key: %s, value: %s\n", filename, lineNumber, key, value);

		if(key[0] == '%') {
			//special command
			trace("Config line is a command: %s\n", key+1);

			if(!strcmp(key+1, "include")) {
				std::string fileToInclude;

				const char *p = filename + strlen(filename);
				while(p >= filename) {
					if(*p == '/' || *p == '\\')
						break;
					p--;
				}
				fileToInclude = std::string(filename, p+1);

				if(value[0] != '$') {
					if(Utils::isAbsolute(value))
						fileToInclude = value;
					else
						fileToInclude += value;
				} else {
					std::string keyStr(value+1);
					ConfigValue* v = getValue(keyStr);
					if(v == nullptr) {
						warn("In %s:%d: Unknown key \"%s\", ignoring\n", filename, lineNumber, keyStr.c_str());
					} else if(v->getType() == ConfigValue::String) {
						std::string keyValue = v->getString();
						if(Utils::isAbsolute(keyValue.c_str()))
							fileToInclude = keyValue;
						else
							fileToInclude += keyValue;
					} else {
						warn("In %s:%d: can\'t include \"%s\", value is not a string\n", filename, lineNumber, keyStr.c_str());
					}
				}

				if(fileToInclude.empty() == false) {
					trace("Including file %s\n", fileToInclude.c_str());
					readFile(fileToInclude.c_str(), fileDepth+1);
				} else {
					trace("Not including a file, filename is empty: \"%s\"\n", fileToInclude.c_str());
				}
			}
		} else {
			std::string keyStr;

			if(key[0] == '!') {
				createValue<cval>((const char*)key+1, (const char*)value);
				trace("Created config parameter: %s\n", key+1);
				keyStr = key+1;
			} else {
				keyStr = key;
				trace("Config line is a config parameter: %s\n", key);
			}



			ConfigValue* v = getValue(keyStr);
			if(v == nullptr) {
				warn("In %s:%d: Unknown key \"%s\", ignoring\n", filename, lineNumber, keyStr.c_str());
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
