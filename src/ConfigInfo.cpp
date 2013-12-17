#include "ConfigInfo.h"
#include <stdlib.h>
#include <stdio.h>

ConfigValue::ConfigValue(Type type) : type(type) {

}

bool ConfigValue::get(bool* val, bool def) {
	switch(getType()) {
		case Bool:
			*val = data.b;
			break;

		case Integer:
			if(data.n)
				*val = true;
			else
				*val = false;
			break;

		case Float:
			if(data.f)
				*val = true;
			else
				*val = false;
			break;

		case String:
			if(data.s.size() == 0 || data.s.find("false") || data.s.find("0"))
				*val = false;
			else if(data.s.find("true") || data.s.find("1"))
				*val = true;
			break;

		case None:
			*val = def;
			return false;
	}

	return true;
}

bool ConfigValue::get(int* val, int def) {
	switch(getType()) {
		case Bool:
			*val = data.b;
			break;

		case Integer:
			*val = data.n;
			break;

		case Float:
			*val = data.f;
			break;

		case String:
			*val = atoi(data.s.c_str());
			break;

		case None:
			*val = def;
			return false;
	}

	return true;
}

bool ConfigValue::get(float* val, float def) {
	switch(getType()) {
		case Bool:
			*val = data.b;
			break;

		case Integer:
			*val = data.n;
			break;

		case Float:
			*val = data.f;
			break;

		case String:
			*val = atof(data.s.c_str());
			break;

		case None:
			*val = def;
			return false;
	}

	return true;
}

bool ConfigValue::get(std::string* val, const std::string& def) {
	switch(getType()) {
		case Bool:
			*val = std::to_string(data.b);
			break;

		case Integer:
			*val = std::to_string(data.n);
			break;

		case Float:
			*val = std::to_string(data.f);
			break;

		case String:
			*val = data.s;
			break;

		case None:
			*val = def;
			return false;
	}

	return true;
}


void ConfigValue::set(bool val) {
	if(type == None)
		type = Bool;

	switch(getType()) {
		case Bool:
			data.b = val;
			break;

		case Integer:
			data.n = val;
			break;

		case Float:
			data.f = val ? 1.0 : 0.0;
			break;

		case String:
			if(val)
				data.s = std::string("true");
			else
				data.s = std::string("false");
			break;

		case None:
			break;
	}
}

void ConfigValue::set(int val) {
	if(type == None)
		type = Integer;

	switch(getType()) {
		case Bool:
			data.b = val ? true : false;
			break;

		case Integer:
			data.n = val;
			break;

		case Float:
			data.f = val;
			break;

		case String:
			data.s = std::to_string(val);
			break;

		case None:
			break;
	}
}

void ConfigValue::set(float val) {
	if(type == None)
		type = Float;

	switch(getType()) {
		case Bool:
			data.b = val ? true : false;
			break;

		case Integer:
			data.n = val;
			break;

		case Float:
			data.f = val;
			break;

		case String:
			data.s = std::to_string(val);
			break;

		case None:
			break;
	}
}

void ConfigValue::set(const std::string& val) {
	if(type == None)
		type = String;

	switch(getType()) {
		case Bool:
			if(val.size() == 0 || val.find("false") || val.find("0"))
				data.b = false;
			else
				data.b = true;
			break;

		case Integer:
			data.n = atoi(val.c_str());
			break;

		case Float:
			data.f = atof(val.c_str());
			break;

		case String:
			data.s = val;
			break;

		case None:
			break;
	}
}

bool* ConfigValue::getBoolPtr(bool def) {
	if(type == None) {
		type = Bool;
		data.b = def;
	}

	if(type == Bool)
		return &data.b;
	return nullptr;
}

int* ConfigValue::getIntPtr(int def) {
	if(type == None) {
		type = Integer;
		data.n = def;
	}

	if(type == Integer)
		return &data.n;
	return nullptr;
}

float* ConfigValue::getFloatPtr(float def) {
	if(type == None) {
		type = Float;
		data.f = def;
	}

	if(type == Float)
		return &data.f;
	return nullptr;
}

std::string* ConfigValue::getStringPtr(const std::string& def) {
	if(type == None) {
		type = String;
		data.s = def;
	}

	if(type == String)
		return &data.s;
	return nullptr;
}

ConfigInfo::ConfigInfo() : nullValue(ConfigValue::None) {

}

ConfigValue* ConfigInfo::get(const std::string& key, bool createIfNonExistant) {
	std::unordered_map<std::string, ConfigValue*>::const_iterator it;

	it = config.find(key);
	if(it != config.cend()) {
		return it->second;
	} else {
		if(createIfNonExistant) {
			ConfigValue* v = new ConfigValue(ConfigValue::None);
			config.emplace(key, v);
			return v;
		} else {
			return nullptr;
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
	if(!file)
		return false;

	while(fgets(line, 1024, file)) {
		int len = strlen(line);
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
				v->set(std::string(p));
				break;

			case 'N':
			case 'I':
				v = new ConfigValue(ConfigValue::Integer);
				v->set(std::string(p));
				break;

			case 'F':
				v = new ConfigValue(ConfigValue::Float);
				v->set(std::string(p));
				break;

			case 'S':
				v = new ConfigValue(ConfigValue::String);
				v->set(std::string(p));
				break;

			default:
				continue;
		}
		std::pair<Iterator, bool> it = config.emplace(std::string(line+2), v);
		if(it.second == false) {
			delete it.first->second;
			it.first->second = v;
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
	dump(file);
	fclose(file);

	return true;
}

void ConfigInfo::dump(FILE *out) {
	std::unordered_map<std::string, ConfigValue*>::const_iterator it, itEnd;
	std::string val;

	for(it = config.cbegin(), itEnd = config.cend(); it != itEnd; ++it) {
		ConfigValue* v = it->second;
		char type = 'd';
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

			case ConfigValue::None:
				type = '0';
				break;
		}

		v->get(&val, std::string("<NULL>"));

		fprintf(out, "%c %s:%s\n", type, it->first.c_str(), val.c_str());
	}
}
