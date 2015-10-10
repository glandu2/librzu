#include "ConfigValue.h"
#include "Core/Utils.h"
#include <stdlib.h>

ConfigValue::ConfigValue(Type type)
	: keyName(nullptr), type(type), hidden(false)
{
}

char ConfigValue::getTypeLetter() {
	switch(type) {
		case ConfigValue::Bool: return 'B';
		case ConfigValue::Integer: return 'N';
		case ConfigValue::Float: return 'F';
		case ConfigValue::String: return 'S';
	}

	return 'd';
}


bool ConfigValue::getBool() {
	return getBool(false);
}

int ConfigValue::getInt() {
	return getInt(0);
}

float ConfigValue::getFloat() {
	return getFloat(0.0);
}

std::string ConfigValue::getString() {
	switch(getType()) {
		case ConfigValue::Bool:
			return getBool() ? "true" : "false";

		case ConfigValue::Integer:
			return Utils::convertToString(getInt());

		case ConfigValue::Float:
			return Utils::convertToString(getFloat());

		case ConfigValue::String:
			return getString(nullptr);
	}

	return getString(nullptr);
}


bool ConfigValue::getBool(bool def) {
	wrongType(Bool);
	return def;
}

int ConfigValue::getInt(int def) {
	wrongType(Integer);
	return def;
}

float ConfigValue::getFloat(float def) {
	wrongType(Float);
	return def;
}

std::string ConfigValue::getString(const char* def) {
	wrongType(String);
	if(def)
		return std::string(def);
	else
		return std::string();
}

std::string ConfigValue::getString(const std::string& def) {
	wrongType(String);
	return def;
}


void ConfigValue::setBool(bool val) {
	wrongType(Bool);
}

void ConfigValue::setInt(int val) {
	wrongType(Integer);
}

void ConfigValue::setFloat(float val) {
	wrongType(Float);
}

void ConfigValue::setString(const char* val) {
	switch(type) {
		case ConfigValue::Bool:
			setBool(!strcmp(val, "true") || !strcmp(val, "1"));
			break;

		case ConfigValue::Integer:
			setInt(atoi(val));
			break;

		case ConfigValue::Float:
			setFloat((float)atof(val));
			break;

		case ConfigValue::String:
			setString(std::string(val));
			break;
	}
}

void ConfigValue::setString(const std::string& val) {
	wrongType(String);
}

bool ConfigValue::isDefault() {
	return false;
}

void ConfigValue::wrongType(Type expectedType) {
	if(expectedType != type) {
		static const char* typeStr[] = {
			"Bool",
			"Integer",
			"Float",
			"String"
		};

		log(LL_Fatal, "Config value type mismatch for %s[%s], tried to get as %s. Check the config file.\n",
				keyName != nullptr ? keyName->c_str() : "unknown key",
				typeStr[type],
				typeStr[expectedType]);
		abort();
	}
}
