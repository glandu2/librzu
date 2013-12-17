#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include "Object.h"
#include <unordered_map>
#include <string>
#include <stdio.h>

class ConfigValue
{
public:
	enum Type {
		Bool,
		Integer,
		Float,
		String,
		None
	};

	ConfigValue(Type type);

	Type getType() { return type; }

	bool get(bool* val, bool def = false);
	bool get(int* val, int def = 0);
	bool get(float* val, float def = 0);
	bool get(std::string* val, const std::string& def = std::string());

	bool get(bool def = false) { bool val; get(&val, def); return val; }
	int get(int def = 0) { int val; get(&val, def); return val; }
	float get(float def = 0) { float val; get(&val, def); return val; }
	std::string get(const std::string& def = std::string()) { std::string val; get(&val, def); return val; }
	std::string get(const char* def = "") { std::string val; get(&val, std::string(def)); return val; }

	void set(bool val);
	void set(int val);
	void set(float val);
	void set(const std::string& val);

	bool* getBoolPtr(bool def = false);
	int* getIntPtr(int def = 0);
	float* getFloatPtr(float def = 0);
	std::string* getStringPtr(const std::string& def = std::string());

private:
	Type type;
	struct {
		bool b;
		int n;
		float f;
		std::string s;
	} data;

};

class ConfigInfo : public Object
{
	DECLARE_CLASS(ConfigInfo)

public:
	ConfigInfo();

	bool readFile(const char *filename);
	bool writeFile(const char* filename);
	void dump(FILE* out);

	ConfigValue* get(const std::string& key, bool createIfNonExistant = true);

	static ConfigInfo* get() {
		static ConfigInfo instance;
		return &instance;
	}

private:
	std::unordered_map<std::string, ConfigValue*> config;
	ConfigValue nullValue;

};


#endif // CONFIGINFO_H
