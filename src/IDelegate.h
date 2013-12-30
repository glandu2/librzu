#ifndef IDELEGATE_H
#define IDELEGATE_H

#include <unordered_map>
#include <tuple>
#include "ICallbackGuard.h"

template<typename T>
struct Callback {
	ICallbackGuard* instance;
	T callback;

	Callback(ICallbackGuard* instance = nullptr, T callback = nullptr) : instance(instance), callback(callback) {
		if(instance)
			instance->addInstance((DelegateRef)&this->callback);
	}
};
#define CALLBACK_CALL(c, ...) \
	do { if((c).callback != nullptr) (c).callback((c).instance, __VA_ARGS__); } while(0)

template<class Key, typename CallbackType>
class IDelegateHash {
public:
	//typedef void (*CallbackType)(ICallbackGuard* instance, Values...);
	struct CallbackInfo {
		CallbackInfo(ICallbackGuard* instance, CallbackType callback) : instance(instance), callback(callback) {}
		ICallbackGuard *instance;
		CallbackType callback;
	};
	typedef typename std::unordered_map<Key, CallbackInfo>::const_iterator CallbackIterator;

	void add(Key key, ICallbackGuard* instance, CallbackType callback) {
		typename std::unordered_map<Key, CallbackInfo>::iterator it;
		it = callbacks.insert(std::pair<Key, CallbackInfo>(key, CallbackInfo(instance, callback)));
		if(instance)
			instance->addInstance((DelegateRef)&(it->second.callback));
	}

	void del(DelegateRef ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(Key key) {
		callbacks.erase(key);
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
				callbackInfo.callback(callbackInfo.instance, __VA_ARGS__); \
				++it; \
			} else { \
				it = (c).callbacks.erase(it); \
			} \
		} \
	} while(0)


	std::unordered_multimap<Key, CallbackInfo> callbacks;
};

template<typename CallbackType>
class IDelegate {
public:
	//typedef void (*CallbackType)(ICallbackGuard* instance, Values...);
	typedef typename std::unordered_map<ICallbackGuard*, CallbackType>::const_iterator CallbackIterator;

	void add(ICallbackGuard* instance, CallbackType callback) {
		typename std::unordered_map<ICallbackGuard*, CallbackType>::iterator it;
		it = callbacks.insert(std::pair<ICallbackGuard*, CallbackType>(instance, callback)).first;
		if(instance)
			instance->addInstance((DelegateRef)&(it->second));
	}

	void del(DelegateRef ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(ICallbackGuard* key) {
		callbacks.erase(key);
	}

#define DELEGATE_CALL(c, ...) \
	do { \
		typedef decltype(c) DelegateType;\
		DelegateType::CallbackIterator it, itEnd; \
 \
		for(it = (c).callbacks.cbegin(), itEnd = (c).callbacks.cend(); it != itEnd;) { \
			ICallbackGuard* instance = it->first; \
			auto callback = it->second; \
 \
			if(callback != nullptr) { \
				callback(instance, __VA_ARGS__); \
				++it; \
			} else { \
				it = (c).callbacks.erase(it); \
			} \
		} \
	} while(0)


	std::unordered_map<ICallbackGuard*, CallbackType> callbacks;
};

#endif // IDELEGATE_H
