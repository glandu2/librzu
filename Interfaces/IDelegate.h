#ifndef IDELEGATE_H
#define IDELEGATE_H

#include <unordered_map>
#include <tuple>

typedef void** DelegateInvalidatePtr;

template<class Key, typename ...Values>
class IDelegateHash {
public:
	typedef void (*CallbackType)(void* instance, Values...);
	struct CallbackInfo {
		void *instance;
		CallbackType callback;
	};

	DelegateInvalidatePtr add(Key key, void* instance, CallbackType callback) {
		typename std::unordered_map<Key, CallbackInfo>::iterator it;
		it = callbacks.emplace(key, CallbackInfo({instance, callback}));
		return (DelegateInvalidatePtr)&(it->second.callback);
	}

	void del(DelegateInvalidatePtr ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(Key key) {
		callbacks.erase(key);
	}

	void reserve(typename std::unordered_multimap<Key, CallbackInfo>::size_type n) {
		callbacks.reserve(n);
	}

	void dispatch(Key key, Values... args) {
		typedef typename std::unordered_map<Key, CallbackInfo>::const_iterator CallbackIterator;
		std::pair<CallbackIterator, CallbackIterator> callbackIterators = callbacks.equal_range(key);
		CallbackIterator it;

		for(it = callbackIterators.first; it != callbackIterators.second;) {
			const CallbackInfo& callbackInfo = it->second;

			if(callbackInfo.callback != nullptr) {
				callbackInfo.callback(callbackInfo.instance, args...);
				++it;
			} else {
				it = callbacks.erase(it);
			}
		}
	}

private:
	std::unordered_multimap<Key, CallbackInfo> callbacks;
};



template<typename ...Values>
class IDelegate {
public:
	typedef void (*CallbackType)(void* instance, Values...);

	DelegateInvalidatePtr add(void* instance, CallbackType callback) {
		typename std::unordered_map<void*, CallbackType>::iterator it;
		it = callbacks.emplace(instance, callback).first;
		return (DelegateInvalidatePtr)&it->second;
	}

	void del(DelegateInvalidatePtr ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(void* key) {
		callbacks.erase(key);
	}

	void dispatch(Values... args) {
		typedef typename std::unordered_map<void*, CallbackType>::const_iterator CallbackIterator;
		CallbackIterator it, itEnd;

		for(it = callbacks.cbegin(), itEnd = callbacks.cend(); it != itEnd;) {
			void* instance = it->first;
			const CallbackType& callback = it->second;

			if(callback != nullptr) {
				callback(instance, args...);
				++it;
			} else {
				it = callbacks.erase(it);
			}
		}
	}

private:
	std::unordered_map<void*, CallbackType> callbacks;
};

#endif // IDELEGATE_H
