#ifndef CONFIGVAL_H
#define CONFIGVAL_H

#include "ConfigValue.h"
#include <string>
#include <list>
#include <type_traits>
#include "uv.h"
#include "IDelegate.h"

template<typename T>
class cval : public ConfigTypedValue<T> {
public:
	typedef void (*EventCallback)(IListener* instance);
	typedef void (*EventCallbackWithThis)(IListener* instance, cval<T>* value);

	cval() : _isDefault(true) { uv_mutex_init(&lock); uv_mutex_init(&listenersLock); }
	cval(const T& value) : value(value), _isDefault(true)  { uv_mutex_init(&lock); uv_mutex_init(&listenersLock); }

	T get() {
		T val;

		uv_mutex_lock(&lock);
		val = value;
		uv_mutex_unlock(&lock);

		return val;
	}

	T get(const T& def) {
		T val;

		uv_mutex_lock(&lock);
		if(_isDefault)
			val = def;
		else
			val = value;
		uv_mutex_unlock(&lock);

		return val;
	}

	void set(const T& val, bool dispatch = true) {
		uv_mutex_lock(&lock);
		value = val;
		_isDefault = false;
		uv_mutex_unlock(&lock);

		if(dispatch)
			dispatchValueChanged();
	}

	bool setDefault(const T& def, bool dispatch = true) {
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
	}

	operator T() { return get(); }
	cval<T>& operator=(const T& val) { set(val); return *this; }

	bool isDefault() { return _isDefault; }

	void addListener(IListener* instance, EventCallback callback) {
		uv_mutex_lock(&listenersLock);
		listeners.push_back(Callback<EventCallback>(instance, callback));
		uv_mutex_unlock(&listenersLock);
	}

	void addListener(IListener* instance, EventCallbackWithThis callback) {
		uv_mutex_lock(&listenersLock);
		listenersWithThis.push_back(Callback<EventCallbackWithThis>(instance, callback));
		uv_mutex_unlock(&listenersLock);
	}

	void dispatchValueChanged() {
		std::list< Callback<EventCallback> > listenersCopy;
		std::list< Callback<EventCallbackWithThis> > listenersWithThisCopy;

		uv_mutex_lock(&listenersLock);
		listenersCopy = listeners;
		listenersWithThisCopy = listenersWithThis;
		uv_mutex_unlock(&listenersLock);

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
	uv_mutex_t lock;
	uv_mutex_t listenersLock;
	bool _isDefault;
};



#endif // CONFIGVAL_H
