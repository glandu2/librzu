#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include "Object.h"
#include <unordered_map>
#include <string>
#include <stdio.h>

class RAPPELZLIB_EXTERN ConfigValue
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
	void setKeyName(const std::string* keyName) { this->keyName = keyName; }
	bool check(Type expectedType, bool soft);

	bool get(bool def) { check(Bool, true); if(type == Bool) return data.b; else return def; }
	int get(int def) { check(Integer, true); if(type == Integer) return data.n; else return def; }
	float get(float def) { check(Float, true); if(type == Float) return data.f; else return def; }
	const std::string& get(const std::string& def) { check(String, true); if(type == String) return data.s; else return def; }
	std::string get(const char* def) { check(String, true); if(type == String) return data.s; else return std::string(def); }

	void set(bool val) { if(type == None) type = Bool; if(check(Bool, true)) data.b = val; }
	void set(int val) { if(type == None) type = Integer; if(check(Integer, true)) data.n = val; }
	void set(float val) { if(type == None) type = Float; if(check(Float, true)) data.f = val; }
	void set(const std::string& val) { if(type == None) type = String; if(check(String, true)) data.s = val; }
	void set(const ConfigValue* v) { type = v->type; data = v->data; }
	void set(double val) { set((float)val); }
	void set(const char* val) { set(std::string(val)); }

	bool& getRef(bool def) { if(type == None) { type = Bool; data.b = def; } check(Bool, false); return data.b; }
	int& getRef(int def) { if(type == None) { type = Integer; data.n = def; } check(Integer, false); return data.n; }
	float& getRef(float def) { if(type == None) { type = Float; data.f = def; } check(Float, false); return data.f; }
	std::string& getRef(const std::string& def) { if(type == None) { type = String; data.s = def; } check(String, false); return data.s; }
	std::string& getRef(const char* def) { if(type == None) { type = String; data.s = def; } check(String, false); return data.s; }

	bool* getPtr(bool def) { if(type == None) { type = Bool; data.b = def; } check(Bool, false); return &data.b; }
	int* getPtr(int def) { if(type == None) { type = Integer; data.n = def; } check(Integer, false); return &data.n; }
	float* getPtr(float def) { if(type == None) { type = Float; data.f = def; } check(Float, false); return &data.f; }
	std::string* getPtr(const std::string& def) { if(type == None) { type = String; data.s = def; } check(String, false); return &data.s; }
	std::string* getPtr(const char* def) { if(type == None) { type = String; data.s = def; } check(String, false); return &data.s; }

private:
	Type type;
	const std::string* keyName;
	struct {
		bool b;
		int n;
		float f;
		std::string s;
	} data;

};

class RAPPELZLIB_EXTERN ConfigInfo : public Object
{
	DECLARE_CLASS(ConfigInfo)

public:
	ConfigInfo();

	bool readFile(const char *filename);
	bool writeFile(const char* filename);
	void dump(FILE* out);

	ConfigValue* get(const std::string& key);

	static bool get(const char* key, bool def) { return ConfigInfo::get()->get(key)->get(def); }
	static int get(const char* key, int def) { return ConfigInfo::get()->get(key)->get(def); }
	static float get(const char* key, float def) { return ConfigInfo::get()->get(key)->get(def); }
	static std::string get(const char* key, const std::string& def) { return ConfigInfo::get()->get(key)->get(def); }
	static std::string get(const char* key, const char* def) { return ConfigInfo::get()->get(key)->get(def); }

	static bool& getRef(const char* key, bool def) { return ConfigInfo::get()->get(key)->getRef(def); }
	static int& getRef(const char* key, int def) { return ConfigInfo::get()->get(key)->getRef(def); }
	static float& getRef(const char* key, float def) { return ConfigInfo::get()->get(key)->getRef(def); }
	static std::string& getRef(const char* key, const std::string& def) { return ConfigInfo::get()->get(key)->getRef(def); }
	static std::string& getRef(const char* key, const char* def) { return ConfigInfo::get()->get(key)->getRef(def); }

	static bool* getPtr(const char* key, bool def) { return ConfigInfo::get()->get(key)->getPtr(def); }
	static int* getPtr(const char* key, int def) { return ConfigInfo::get()->get(key)->getPtr(def); }
	static float* getPtr(const char* key, float def) { return ConfigInfo::get()->get(key)->getPtr(def); }
	static std::string* getPtr(const char* key, const std::string& def) { return ConfigInfo::get()->get(key)->getPtr(def); }
	static std::string* getPtr(const char* key, const char* def) { return ConfigInfo::get()->get(key)->getPtr(def); }

	static ConfigInfo* get() {
		static ConfigInfo instance;
		return &instance;
	}

private:
	std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> addValue(const std::string& key, ConfigValue* v);
	std::unordered_map<std::string, ConfigValue*> config;

};

#define CFG(key, defaultValue) ConfigInfo::getRef(key, defaultValue)

#endif // CONFIGINFO_H
