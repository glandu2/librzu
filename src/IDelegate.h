#ifndef IDELEGATE_H
#define IDELEGATE_H

#include <unordered_map>
#include <tuple>
#include "IListener.h"

template<typename T>
struct Callback {
	IListener* instance;
	T callback;

	Callback(const Callback& other) : instance(other.instance), callback(other.callback) {}

	Callback(Callback&& other) : instance(other.instance), callback(other.callback) {
		other.instance = nullptr;
		other.callback = nullptr;
	}

	Callback(IListener* instance = nullptr, T callback = nullptr) : instance(instance), callback(callback) {
		if(instance)
			instance->addDelegateRef((DelegateRef)&this->callback);
	}

	Callback& operator=(const Callback& other) {
		instance = other.instance;
		callback = other.callback;

		return *this;
	}

	~Callback() {
		if(instance && callback)
			instance->delDelegateRef((DelegateRef)&callback);
	}
};
#define CALLBACK_CALL(c, ...) \
	do { if((c).callback != nullptr) (c).callback((c).instance, ## __VA_ARGS__); } while(0)

template<class Key, typename CallbackType>
class IDelegateHash {
public:
	//typedef void (*CallbackType)(ICallbackGuard* instance, Values...);
	struct CallbackInfo {
		CallbackInfo(IListener* instance, CallbackType callback) : instance(instance), callback(callback) {}
		IListener *instance;
		CallbackType callback;
	};
	typedef typename std::unordered_map<Key, CallbackInfo>::iterator CallbackIterator;

	IDelegateHash() : callDepth(0) {}
	IDelegateHash(const IDelegateHash& other) : callbacks(other.callbacks), callDepth(0) {}
	IDelegateHash(IDelegateHash&& other) : callbacks(std::move(other.callbacks)), callDepth(0) {}

	~IDelegateHash() {
		CallbackIterator it;
		for(it = callbacks.begin(); it != callbacks.end();) {
			const CallbackInfo& callbackInfo = it->second;

			if(callbackInfo.instance && callbackInfo.callback)
				callbackInfo.instance->delDelegateRef((DelegateRef)&callbackInfo.callback);

			it = callbacks.erase(it);
		}
	}

	IDelegateHash& operator=(const IDelegateHash& other) {
		callbacks = other.callbacks;

		return *this;
	}

	void add(Key key, IListener* instance, CallbackType callback) {
		CallbackIterator it;
		it = callbacks.insert(std::pair<Key, CallbackInfo>(key, CallbackInfo(instance, callback)));
		if(instance)
			instance->addDelegateRef((DelegateRef)&(it->second.callback));
	}

	void del(Key key) {
		CallbackIterator it;
		std::pair<CallbackIterator, CallbackIterator> range = callbacks.equal_range(key);
		for(it = range.first; it != range.second; ++it) {
			const CallbackInfo& callbackInfo = it->second;

			if(callbackInfo.instance && callbackInfo.callback)
				callbackInfo.instance->delDelegateRef((DelegateRef)&callbackInfo.callback);

			callbackInfo.callback = nullptr; //differed del
		}
	}

	void del(Key key, IListener* instance) {
		CallbackIterator it;
		std::pair<CallbackIterator, CallbackIterator> range = callbacks.equal_range(key);
		for(it = range.first; it != range.second; ++it) {
			CallbackInfo& callbackInfo = it->second;

			if(callbackInfo.instance == instance) {
				if(callbackInfo.instance && callbackInfo.callback)
					callbackInfo.instance->delDelegateRef((DelegateRef)&callbackInfo.callback);

				callbackInfo.callback = nullptr; //differed del
			}
		}
	}

	void reserve(typename std::unordered_multimap<Key, CallbackInfo>::size_type n) {
#ifndef _MSC_VER
		callbacks.reserve(n);
#endif
	}

#define DELEGATE_HASH_CALL(c, key, ...) \
	do { \
		typedef decltype(c) DelegateType;\
		std::pair<DelegateType::CallbackIterator, DelegateType::CallbackIterator> callbackIterators = (c).callbacks.equal_range(key); \
		DelegateType::CallbackIterator it; \
 \
		for(it = callbackIterators.first; it != callbackIterators.second;) { \
			const DelegateType::CallbackInfo& callbackInfo = it->second; \
 \
			if(callbackInfo.callback != nullptr) { \
				(c).callDepth++; \
				callbackInfo.callback(callbackInfo.instance, __VA_ARGS__); \
				(c).callDepth--; \
				++it; \
			} else if((c).callDepth == 0) { \
				it = (c).callbacks.erase(it); \
			} else { \
				++it; \
			} \
		} \
	} while(0)


	std::unordered_multimap<Key, CallbackInfo> callbacks;
	int callDepth; //purge removed callbacks only at depth 0
};

template<typename CallbackType>
class IDelegate {
public:
	//typedef void (*CallbackType)(ICallbackGuard* instance, Values...);
	typedef typename std::unordered_map<IListener*, CallbackType>::iterator CallbackIterator;

	IDelegate() : callDepth(0) {}
	IDelegate(IDelegate& other) : callbacks(other.callbacks), callDepth(0) {}
	IDelegate(IDelegate&& other) : callbacks(std::move(other.callbacks)), callDepth(0) {}

	~IDelegate() {
		CallbackIterator it;
		for(it = callbacks.begin(); it != callbacks.end();) {
			IListener* instance = it->first;
			const CallbackType& callbackInfo = it->second;

			if(instance && callbackInfo)
				instance->delDelegateRef((DelegateRef)&callbackInfo);

			it = callbacks.erase(it);
		}
	}

	IDelegate& operator=(const IDelegate& other) {
		callbacks = other.callbacks;

		return *this;
	}

	void add(IListener* instance, CallbackType callback) {
		CallbackIterator it;
		it = callbacks.insert(std::pair<IListener*, CallbackType>(instance, callback)).first;
		if(instance)
			instance->addDelegateRef((DelegateRef)&(it->second));
	}

	void del(IListener* key) {
		CallbackIterator it = callbacks.find(key);
		if(key && it->second)
			key->delDelegateRef((DelegateRef)&(it->second));
		it->second = nullptr;
	}

#define DELEGATE_CALL(c, ...) \
	do { \
		typedef decltype(c) DelegateType;\
		DelegateType::CallbackIterator it, itEnd; \
 \
		for(it = (c).callbacks.begin(), itEnd = (c).callbacks.end(); it != itEnd;) { \
			IListener* instance = it->first; \
			auto callback = it->second; \
 \
			if(callback != nullptr) { \
				(c).callDepth++; \
				callback(instance, __VA_ARGS__); \
				(c).callDepth--; \
				++it; \
			} else if((c).callDepth == 0) { \
				it = (c).callbacks.erase(it); \
			} else { \
				++it; \
			} \
		} \
	} while(0)


	std::unordered_map<IListener*, CallbackType> callbacks;
	int callDepth; //purge removed callbacks only at depth 0
};

#endif // IDELEGATE_H
