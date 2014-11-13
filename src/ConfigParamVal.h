#ifndef CONFIGPARAMVAL_H
#define CONFIGPARAMVAL_H

#include "ConfigValue.h"
#include <string>
#include <list>
#include <type_traits>
#include "uv.h"
#include "IDelegate.h"

//#define NOMUTEX

template<typename T>
class cval : public ConfigTypedValue<T> {
public:
	typedef void (*EventCallback)(IListener* instance);
	typedef void (*EventCallbackWithThis)(IListener* instance, cval<T>* value);

	cval() : _isDefault(true) {
#ifndef NOMUTEX
		uv_mutex_init(&lock);
		uv_mutex_init(&listenersLock);
#endif
	}
	cval(const T& value) : value(value), _isDefault(true) {
#ifndef NOMUTEX
		uv_mutex_init(&lock);
		uv_mutex_init(&listenersLock);
#endif
	}

	T get() {
#ifdef NOMUTEX
		return value;
#else
		T val;

		uv_mutex_lock(&lock);
		val = value;
		uv_mutex_unlock(&lock);

		return val;
#endif
	}

	T get(const T& def) {
#ifdef NOMUTEX
		if(_isDefault)
			return def;
		else
			return value;
#else
		T val;

		uv_mutex_lock(&lock);
		if(_isDefault)
			val = def;
		else
			val = value;
		uv_mutex_unlock(&lock);

		return val;
#endif
	}

	void set(const T& val, bool dispatch = true) {
#ifdef NOMUTEX
		value = val;
		_isDefault = false;
#else
		uv_mutex_lock(&lock);
		value = val;
		_isDefault = false;
		uv_mutex_unlock(&lock);
#endif

		if(dispatch)
			dispatchValueChanged();
	}

	bool setDefault(const T& def, bool dispatch = true) {
#ifdef NOMUTEX
		if(_isDefault) {
			value = def;
			if(dispatch)
				dispatchValueChanged();
			return true;
		}
		return false;
#else
		bool changed = false;

		uv_mutex_lock(&lock);
		if(_isDefault) {
			value = def;
			changed = true;
		}
		uv_mutex_unlock(&lock);

		if(changed && dispatch)
			dispatchValueChanged();
		return changed;
#endif
	}

	operator T() { return get(); }
	cval<T>& operator=(const T& val) { set(val); return *this; }

	bool isDefault() { return _isDefault; }

	void addListener(IListener* instance, EventCallback callback) {
#ifndef NOMUTEX
		uv_mutex_lock(&listenersLock);
#endif
		listeners.push_back(Callback<EventCallback>(instance, callback));

#ifndef NOMUTEX
		uv_mutex_unlock(&listenersLock);
#endif
	}

	void addListener(IListener* instance, EventCallbackWithThis callback) {
#ifndef NOMUTEX
		uv_mutex_lock(&listenersLock);
#endif
		listenersWithThis.push_back(Callback<EventCallbackWithThis>(instance, callback));

#ifndef NOMUTEX
		uv_mutex_unlock(&listenersLock);
#endif
	}

	void dispatchValueChanged() {
		std::list< Callback<EventCallback> > listenersCopy;
		std::list< Callback<EventCallbackWithThis> > listenersWithThisCopy;

#ifndef NOMUTEX
		uv_mutex_lock(&listenersLock);
#endif
		listenersCopy = listeners;
		listenersWithThisCopy = listenersWithThis;

#ifndef NOMUTEX
		uv_mutex_unlock(&listenersLock);
#endif

		auto it = listenersCopy.cbegin();
		auto itEnd = listenersCopy.cend();
		for(; it != itEnd; ++it)
			CALLBACK_CALL(*it);

		auto itWithThis = listenersWithThisCopy.cbegin();
		auto itWithThisEnd = listenersWithThisCopy.cend();
		for(; itWithThis != itWithThisEnd; ++itWithThis)
			CALLBACK_CALL(*itWithThis, this);
	}

private:
	std::list< Callback<EventCallback> > listeners;
	std::list< Callback<EventCallbackWithThis> > listenersWithThis;
	T value;
#ifndef NOMUTEX
	uv_mutex_t lock;
	uv_mutex_t listenersLock;
#endif
	bool _isDefault;
};



#endif // CONFIGPARAMVAL_H
