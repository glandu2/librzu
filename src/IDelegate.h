#ifndef IDELEGATE_H
#define IDELEGATE_H

#include <unordered_map>
#include <tuple>
#include <list>
#include "IListener.h"

template<typename T>
struct Callback {
	IListener* instance;
	T callback;

	Callback(const Callback& other) : instance(other.instance), callback(other.callback) {
		if(instance && callback)
			instance->addDelegateRef((DelegateRef)&this->callback);
	}

	Callback(Callback&& other) : instance(other.instance), callback(other.callback) {
		if(instance && callback) {
			instance->delDelegateRef((DelegateRef)&other.callback);
			instance->addDelegateRef((DelegateRef)&this->callback);
		}
		other.instance = nullptr;
		other.callback = nullptr;
	}

	Callback(IListener* instance = nullptr, T callback = nullptr) : instance(instance), callback(callback) {
		if(instance && callback)
			instance->addDelegateRef((DelegateRef)&this->callback);
	}

	Callback& operator=(const Callback& other) {
		instance = other.instance;
		callback = other.callback;
		if(instance && callback)
			instance->addDelegateRef((DelegateRef)&this->callback);

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
	//IDelegateHash(const IDelegateHash& other) : callbacks(other.callbacks), callDepth(0) {}
	//IDelegateHash(IDelegateHash&& other) : callbacks(std::move(other.callbacks)), callDepth(0) {}

	~IDelegateHash() {
		CallbackIterator it;
		for(it = callbacks.begin(); it != callbacks.end();) {
			const CallbackInfo& callbackInfo = it->second;

			if(callbackInfo.instance && callbackInfo.callback)
				callbackInfo.instance->delDelegateRef((DelegateRef)&callbackInfo.callback);

			it = callbacks.erase(it);
		}
	}

	//manage delDelegateRef
//	IDelegateHash& operator=(const IDelegateHash& other) {
//		callbacks = other.callbacks;

//		return *this;
//	}

	//Do not call add() in the same delegate callbacks
	void add(Key key, IListener* instance, CallbackType callback) {
		if(!callback)
			return;

		if(callDepth != 0) {
			PendingAdd pendingAdd = {key, instance, callback};
			pendingAdds.push_back(pendingAdd);
		} else {
			CallbackIterator it;
			it = callbacks.insert(std::pair<Key, CallbackInfo>(key, CallbackInfo(instance, callback)));
			if(instance)
				instance->addDelegateRef((DelegateRef)&(it->second.callback));
		}
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
		auto callbackIterators = (c).callbacks.equal_range(key); \
		auto it = callbackIterators.first; \
 \
		for(; it != callbackIterators.second;) { \
			auto callbackInfo = it->second; \
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
		(c).processPendingAdds(); \
	} while(0)


	void processPendingAdds() {
		if(callDepth != 0)
			return;

		for(auto it = pendingAdds.begin(); it != pendingAdds.end();) {
			const PendingAdd& pendingAdd = *it;
			add(pendingAdd.key, pendingAdd.instance, pendingAdd.callback);

			it = pendingAdds.erase(it);
		}
	}

	std::unordered_multimap<Key, CallbackInfo> callbacks;
	int callDepth; //purge removed callbacks only at depth 0

private:
	IDelegateHash(IDelegateHash& other);
	IDelegateHash(IDelegateHash&& other);
	IDelegateHash& operator=(const IDelegateHash& other);

	struct PendingAdd {
		Key key;
		IListener* instance;
		CallbackType callback;
	};
	std::list<PendingAdd> pendingAdds;
};

template<typename CallbackType>
class IDelegate {
public:
	//typedef void (*CallbackType)(ICallbackGuard* instance, Values...);
	typedef typename std::unordered_map<IListener*, CallbackType>::iterator CallbackIterator;

	IDelegate() : callDepth(0) {}
	//IDelegate(IDelegate& other) : callbacks(other.callbacks), callDepth(0) {}
	//IDelegate(IDelegate&& other) : callbacks(std::move(other.callbacks)), callDepth(0) {}

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

	//manage delDelegateRef
//	IDelegate& operator=(const IDelegate& other) {
//		callbacks = other.callbacks;

//		return *this;
//	}

	//Do not call add() in the same delegate callbacks
	void add(IListener* instance, CallbackType callback) {
		if(callDepth != 0) {
			PendingAdd pendingAdd = {instance, callback};
			pendingAdds.push_back(pendingAdd);
		} else {
			CallbackIterator it;
			it = callbacks.insert(std::pair<IListener*, CallbackType>(instance, callback)).first;
			if(instance)
				instance->addDelegateRef((DelegateRef)&(it->second));
		}
	}

	void del(IListener* key) {
		CallbackIterator it = callbacks.find(key);
		if(key && it->second)
			key->delDelegateRef((DelegateRef)&(it->second));
		it->second = nullptr;
	}

#define DELEGATE_CALL(c, ...) \
	do { \
		auto it = (c).callbacks.begin(); \
		auto itEnd = (c).callbacks.end(); \
 \
		for(; it != itEnd;) { \
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
		(c).processPendingAdds(); \
	} while(0)


	void processPendingAdds() {
		if(callDepth != 0)
			return;

		for(auto it = pendingAdds.begin(); it != pendingAdds.end();) {
			const PendingAdd& pendingAdd = *it;
			add(pendingAdd.instance, pendingAdd.callback);

			it = pendingAdds.erase(it);
		}
	}

	std::unordered_map<IListener*, CallbackType> callbacks;
	int callDepth; //purge removed callbacks only at depth 0

private:
	IDelegate(IDelegate& other);
	IDelegate(IDelegate&& other);
	IDelegate& operator=(const IDelegate& other);

	struct PendingAdd {
		IListener* instance;
		CallbackType callback;
	};
	std::list<PendingAdd> pendingAdds;
};

#endif // IDELEGATE_H
