#include "ConfigInfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Config/GlobalCoreConfig.h"
#include <map>
#include "Console/ConsoleCommands.h"

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

	ConsoleCommands::get()->addCommand("config.get", "get", 1, &commandGetEnv,
									   "Get a config value",
									   "config.get <config> : get <config> value\r\n"
									   "   example : config.get core.log.level\r\n");

	ConsoleCommands::get()->addCommand("config.set", "set", 2, &commandSetEnv,
									   "Set a config value",
									   "config.set <config> <value>: set <config> to <value>\r\n"
									   "   example : config.set core.log.level debug\r\n");
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
		while(!Utils::isalpha(*key) && *key != '\0')
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
			log(LL_Warning, "Unknown key \"%s\", ignoring\n", keyStr.c_str());
			continue;
		}

		switch(v->getType()) {
			case ConfigValue::Bool:
				v->setBool(!strcmp(value, "true") || !strcmp(value, "1"));
				break;

			case ConfigValue::Integer:
				v->setInt(strtol(value, nullptr, 0));
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

bool ConfigInfo::parseLine(char* line, std::string &keyStr, std::string &valueStr) {
	char *p, *key, *value;

	keyStr.clear();
	valueStr.clear();

	size_t len = strlen(line);

	//remove leading spaces
	p = line;
	while(Utils::isspace(*p) && *p)
		p++;

	key = p;
	if(key[0] == '#' || key[0] == '\0')	//a comment or end of line (nothing on the line)
		return false;

	//remove trailing spaces
	p = line + len - 1;
	while(Utils::isspace(*p) && p > key)
		p--;
	if(p == key)
		return false;
	*(p+1) = 0;

	p = strpbrk(key, ":=");
	if(!p)
		return false;
	*p = 0;
	value = p+1;

	keyStr.assign(key);
	valueStr.assign(value);

	return true;
}

bool ConfigInfo::readFile(const char* filename, int fileDepth) {
	FILE* file;
	char line[1024];
	int lineNumber = 0;
	std::string key;
	std::string value;

	if(fileDepth > 10) {
		log(LL_Error, "Too many file inclusions (%d) while reading %s, continuing without going deeper\n", fileDepth, filename);
		return false;
	}

	file = fopen(filename, "rb");
	if(!file) {
		log(LL_Info, "No config file (%s)\n", filename);
		return false;
	}

	log(LL_Info, "Reading config file %s\n", filename);

	while(fgets(line, 1024, file)) {
		lineNumber++;

		if(!parseLine(line, key, value))
			continue;

		if(value.size() > 0 && value[0] == '$') {
			std::string keyStr = value.substr(1);
			ConfigValue* v = getValue(keyStr);
			if(v == nullptr) {
				log(LL_Warning, "In %s:%d: Unknown key \"%s\", ignoring\n", filename, lineNumber, keyStr.c_str());
			} else {
				value = v->getString();
			}
		}

		log(LL_Trace, "%s:%d: Read key: %s, value: %s\n", filename, lineNumber, key.c_str(), value.c_str());

		if(key[0] == '%') {
			//special command
			std::string command = key.substr(1);
			log(LL_Trace, "Config line is a command: %s\n", command.c_str());

			if(command == "include") {
				std::string fileToInclude;

				const char *p = filename + strlen(filename);
				while(p >= filename) {
					if(*p == '/' || *p == '\\')
						break;
					p--;
				}
				fileToInclude = std::string(filename, p+1);

				if(Utils::isAbsolute(value.c_str()))
					fileToInclude = value;
				else
					fileToInclude += value;

				if(fileToInclude.empty() == false) {
					log(LL_Trace, "Including file %s\n", fileToInclude.c_str());
					readFile(fileToInclude.c_str(), fileDepth+1);
				} else {
					log(LL_Trace, "Not including a file, filename is empty: \"%s\"\n", fileToInclude.c_str());
				}
			}
		} else if(key[0] == '!') {
			key.erase(0, 1);
			createValue<cval>(key.c_str(), value);
			log(LL_Trace, "Created config parameter: %s\n", key.c_str());
		} else {
			log(LL_Trace, "Config line is a config parameter: %s\n", key.c_str());

			ConfigValue* v = getValue(key);
			if(v == nullptr) {
				log(LL_Warning, "In %s:%d: Unknown key \"%s\", ignoring\n", filename, lineNumber, key.c_str());
				continue;
			}

			switch(v->getType()) {
				case ConfigValue::Bool:
					v->setBool(!strcmp(value.c_str(), "true") || !strcmp(value.c_str(), "1"));
					break;

				case ConfigValue::Integer:
					v->setInt(strtol(value.c_str(), nullptr, 0));
					break;

				case ConfigValue::Float:
					v->setFloat((float)atof(value.c_str()));
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

void ConfigInfo::dump(bool showDefault) {
	std::map<std::string, ConfigValue*>::const_iterator it, itEnd;

	log(LL_Info, "Config dump:\n");

	std::map<std::string, ConfigValue*> ordered(config.cbegin(), config.cend());

	for(it = ordered.cbegin(), itEnd = ordered.cend(); it != itEnd; ++it) {
		ConfigValue* v = it->second;
		char type = 'd';
		std::string value;

		if(v->isHidden() == false || GlobalCoreConfig::get()->app.showHiddenConfig.get())
			value = v->getString();
		else
			value = "<hidden>";

		switch(v->getType()) {
			case ConfigValue::Bool:
				type = 'B';
				break;

			case ConfigValue::Integer:
				type = 'N';
				break;

			case ConfigValue::Float:
				type = 'F';
				break;

			case ConfigValue::String:
				type = 'S';
				break;
		}
		if(!v->isDefault() || showDefault)
			log(LL_Info, "%c%c%s:%s\n", type, v->isDefault() ? '*' : ' ', it->first.c_str(), value.c_str());
	}
}


void ConfigInfo::commandSetEnv(IWritableConsole* console, const std::vector<std::string>& args) {
	const std::string& variableName = args[0];
	const std::string& value = args[1];

	ConfigValue* v = ConfigInfo::get()->getValue(variableName);

	if(v == nullptr) {
		console->writef("Unknown variable name: %s\r\n", variableName.c_str());
		return;
	}

	switch(v->getType()) {
		case ConfigValue::Bool:
			v->setBool(!strcmp(value.c_str(), "true") || !strcmp(value.c_str(), "1"));
			break;

		case ConfigValue::Integer:
			v->setInt(atoi(value.c_str()));
			break;

		case ConfigValue::Float:
			v->setFloat((float)atof(value.c_str()));
			break;

		case ConfigValue::String:
			v->setString(value);
			break;
	}


	commandGetEnv(console, args);
}

void ConfigInfo::commandGetEnv(IWritableConsole* console, const std::vector<std::string>& args) {
	const std::string& variableName = args[0];

	ConfigValue* v = ConfigInfo::get()->getValue(variableName);

	if(v == nullptr) {
		console->writef("Unknown variable name: %s\r\n", variableName.c_str());
		return;
	}

	std::string val;
	char type = 'U';

	switch(v->getType()) {
		case ConfigValue::Bool:
			type = 'B';
			val = v->getBool() ? "true" : "false";
			break;

		case ConfigValue::Integer:
			type = 'N';
			val = Utils::convertToString(v->getInt());
			break;

		case ConfigValue::Float:
			type = 'F';
			val = Utils::convertToString(v->getFloat());
			break;

		case ConfigValue::String:
			type = 'S';
			val = v->getString();
			break;
	}

	console->writef("%c%c%s:%s\r\n", type, v->isDefault() ? '*' : ' ', variableName.c_str(), val.c_str());
}
