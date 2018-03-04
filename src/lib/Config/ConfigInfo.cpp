#include "ConfigInfo.h"
#include "Config/GlobalCoreConfig.h"
#include "Console/ConsoleCommands.h"
#include "Core/Utils.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ConfigInfo::~ConfigInfo() {
	for(auto it = config.begin(); it != config.end(); ++it) {
		delete it->second;
	}
}

void ConfigInfo::init(int argc, char** argv) {
	GlobalCoreConfig::init();

	parseCommandLine(argc, argv, true);
	readFile(GlobalCoreConfig::get()->app.configfile.get().c_str());
	// Set all keys given on the command line to overwrite config file values
	parseCommandLine(argc, argv);

	ConsoleCommands::get()->addCommand(
	    "config.get",
	    "get",
	    0,
	    1,
	    &commandGetEnv,
	    "Get a config value",
	    "config.get <config> : get <config> value. <config> can contains wildcards (* and ?)\r\n"
	    "   example : config.get core.log.level\r\n");

	ConsoleCommands::get()->addCommand("config.set",
	                                   "set",
	                                   0,
	                                   2,
	                                   &commandSetEnv,
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

std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> ConfigInfo::addValue(const std::string& key,
                                                                                              ConfigValue* v) {
	std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> it =
	    config.insert(std::pair<std::string, ConfigValue*>(key, v));
	if(it.second)
		v->setKeyName(&it.first->first);
	return it;
}

void ConfigInfo::parseCommandLine(int argc, char** argv, bool onlyConfigFileLocation) {
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

		// Check only config file location value if requested
		if(onlyConfigFileLocation && keyStr != CONFIG_FILE_KEY)
			continue;

		value++;
		ConfigValue* v = getValue(keyStr);
		if(v == nullptr) {
			log(LL_Warning, "Unknown key \"%s\", ignoring\n", keyStr.c_str());
			continue;
		}

		v->setString(value);
	}
}

bool ConfigInfo::parseLine(char* line, std::string& keyStr, std::string& valueStr) {
	char *p, *key, *value;

	keyStr.clear();
	valueStr.clear();

	size_t len = strlen(line);

	// remove leading spaces
	p = line;
	while(Utils::isspace(*p) && *p)
		p++;

	key = p;
	if(key[0] == '#' || key[0] == '\0')  // a comment or end of line (nothing on the line)
		return false;

	// remove trailing spaces
	p = line + len - 1;
	while(Utils::isspace(*p) && p > key)
		p--;
	if(p == key)
		return false;
	*(p + 1) = 0;

	p = strpbrk(key, ":=");
	if(!p)
		return false;
	*p = 0;
	value = p + 1;

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
		log(LL_Error,
		    "Too many file inclusions (%d) while reading %s, continuing without going deeper\n",
		    fileDepth,
		    filename);
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
			// special command
			std::string command = key.substr(1);
			log(LL_Trace, "Config line is a command: %s\n", command.c_str());

			if(command == "include") {
				std::string fileToInclude;

				const char* p = filename + strlen(filename);
				while(p >= filename) {
					if(*p == '/' || *p == '\\')
						break;
					p--;
				}
				fileToInclude = std::string(filename, p + 1);

				if(Utils::isAbsolute(value.c_str()))
					fileToInclude = value;
				else
					fileToInclude += value;

				if(fileToInclude.empty() == false) {
					log(LL_Trace, "Including file %s\n", fileToInclude.c_str());
					readFile(fileToInclude.c_str(), fileDepth + 1);
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

			v->setString(value.c_str());
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

		if(v->isHidden() && !GlobalCoreConfig::get()->app.showHiddenConfig.get())
			continue;

		if(!v->isDefault() || showDefault)
			log(LL_Info,
			    "%c%c%s:%s\n",
			    v->getTypeLetter(),
			    v->isDefault() ? '*' : ' ',
			    it->first.c_str(),
			    v->getString().c_str());
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

	v->setString(value.c_str());

	commandGetEnv(console, args);
}

void ConfigInfo::commandGetEnv(IWritableConsole* console, const std::vector<std::string>& args) {
	const char* pattern = args.size() >= 1 ? args[0].c_str() : "*";

	std::map<std::string, ConfigValue*> ordered;

	{
		std::unordered_map<std::string, ConfigValue*>::const_iterator it, itEnd;
		for(it = ConfigInfo::get()->config.cbegin(), itEnd = ConfigInfo::get()->config.cend(); it != itEnd; ++it) {
			if(Utils::stringWildcardMatch(it->first.c_str(), pattern)) {
				ordered.insert(std::pair<std::string, ConfigValue*>(it->first, it->second));
			}
		}
	}

	if(!ordered.empty()) {
		std::map<std::string, ConfigValue*>::const_iterator itOrdered, itEndOrdered;
		for(itOrdered = ordered.cbegin(), itEndOrdered = ordered.cend(); itOrdered != itEndOrdered; ++itOrdered) {
			ConfigValue* v = itOrdered->second;
			console->writef("%c%c%s:%s\r\n",
			                v->getTypeLetter(),
			                v->isDefault() ? '*' : ' ',
			                itOrdered->first.c_str(),
			                v->getString().c_str());
		}
	} else {
		console->writef("No config matching pattern %s\n", pattern);
	}
}
