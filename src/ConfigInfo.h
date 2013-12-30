#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include "Object.h"
#include <unordered_map>
#include <string>
#include <stdio.h>
#include "uv.h"
#include <list>
#include "IDelegate.h"

template<class T>
class cval {
public:
	typedef void (*EventCallback)(ICallbackGuard* instance, cval<T>* value);

	cval() { uv_rwlock_init(&lock); uv_mutex_init(&listenersLock); }
	cval(const T& value) : value(value) { uv_rwlock_init(&lock); uv_mutex_init(&listenersLock); }

	T get() { T val; uv_rwlock_rdlock(&lock); val = value; uv_rwlock_rdunlock(&lock); return val; }
	void set(const T& val) { uv_rwlock_wrlock(&lock); value = val; uv_rwlock_wrunlock(&lock); dispatchValueChanged(); }

	operator T() { return get(); }
	cval<T>& operator=(const T& val) { set(val); return *this; }

	void addListener(ICallbackGuard* instance, EventCallback callback) { listeners.push_back(Callback<EventCallback>(instance, callback)); }
	void dispatchValueChanged() {
		std::list< Callback<EventCallback> > listenersCopy;

		uv_mutex_lock(&listenersLock);
		listenersCopy = listeners;
		uv_mutex_unlock(&listenersLock);

		auto it = listenersCopy.cbegin();
		auto itEnd = listenersCopy.cend();
		for(; it != itEnd; ++it)
			CALLBACK_CALL(*it, this);
	}

private:
	std::list< Callback<EventCallback> > listeners;
	T value;
	uv_rwlock_t lock;
	uv_mutex_t listenersLock;
};

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

	void set(bool val) { if(type == None) type = Bool; if(check(Bool, true)) data.b = val; }
	void set(int val) { if(type == None) type = Integer; if(check(Integer, true)) data.n = val; }
	void set(float val) { if(type == None) type = Float; if(check(Float, true)) data.f = val; }
	void set(const std::string& val) { if(type == None) type = String; if(check(String, true)) data.s = val; }
	void set(const ConfigValue* v) { type = v->type; data = v->data; }
	void set(double val) { set((float)val); }
	void set(const char* val) { set(std::string(val)); }

	cval<bool>& get(bool def) { if(type == None) { type = Bool; data.b = def; } check(Bool, false); return data.b; }
	cval<int>& get(int def) { if(type == None) { type = Integer; data.n = def; } check(Integer, false); return data.n; }
	cval<float>& get(float def) { if(type == None) { type = Float; data.f = def; } check(Float, false); return data.f; }
	cval<std::string>& get(const std::string& def) { if(type == None) { type = String; data.s = def; } check(String, false); return data.s; }
	cval<std::string>& get(const char* def) { if(type == None) { type = String; data.s = def; } check(String, false); return data.s; }

private:
	Type type;
	const std::string* keyName;
	struct {
		cval<bool> b;
		cval<int> n;
		cval<float> f;
		cval<std::string> s;
	} data;

};

class RAPPELZLIB_EXTERN ConfigInfo : public Object
{
	DECLARE_CLASS(ConfigInfo)

public:
	ConfigInfo();

	void parseCommandLine(int argc, char **argv);
	bool readFile(const char *filename);
	bool writeFile(const char* filename);
	void dump(FILE* out);

	ConfigValue* getValue(const std::string& key, bool createIfNonExistant = true);

	static cval<bool>& get(const char* key, bool def) { return ConfigInfo::get()->getValue(key)->get(def); }
	static cval<int>& get(const char* key, int def) { return ConfigInfo::get()->getValue(key)->get(def); }
	static cval<float>& get(const char* key, float def) { return ConfigInfo::get()->getValue(key)->get(def); }
	static cval<std::string>& get(const char* key, const std::string& def) { return ConfigInfo::get()->getValue(key)->get(def); }
	static cval<std::string>& get(const char* key, const char* def) { return ConfigInfo::get()->getValue(key)->get(def); }

	static ConfigInfo* get() {
		static ConfigInfo instance;
		return &instance;
	}

	static void init() {
		ConfigInfo::get();
	}

private:
	std::pair<std::unordered_map<std::string, ConfigValue*>::iterator, bool> addValue(const std::string& key, ConfigValue* v);
	std::unordered_map<std::string, ConfigValue*> config;

};

#define CFG(key, defaultValue) ConfigInfo::get(key, defaultValue)

#endif // CONFIGINFO_H
