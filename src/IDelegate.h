#ifndef IDELEGATE_H
#define IDELEGATE_H

#include <unordered_map>
#include <tuple>
#include "ICallbackGuard.h"

template<typename T>
struct Callback {
	void* instance;
	T callback;

	Callback(void* instance = nullptr, T callback = nullptr) : instance(instance), callback(callback) {}
};
#define CALLBACK_CALL(c, ...) (c).callback((c).instance, __VA_ARGS__)

#if !defined(_MSC_VER) || _MSC_FULL_VER >= 170051025
template<class Key, typename ...Values>
class IDelegateHash {
public:
	typedef void (*CallbackType)(void* instance, Values...);
	struct CallbackInfo {
		void *instance;
		CallbackType callback;
	};

	DelegateRef add(Key key, void* instance, CallbackType callback) {
		typename std::unordered_map<Key, CallbackInfo>::iterator it;
		it = callbacks.emplace(key, CallbackInfo({instance, callback}));
		return (DelegateRef)&(it->second.callback);
	}

	void del(DelegateRef ptr) {
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

	DelegateRef add(void* instance, CallbackType callback) {
		typename std::unordered_map<void*, CallbackType>::iterator it;
		it = callbacks.emplace(instance, callback).first;
		return (DelegateRef)&it->second;
	}

	void del(DelegateRef ptr) {
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

#else

template<class Key, typename V1, typename V2>
class IDelegateHash {
public:
	typedef void (*CallbackType)(void*, V1, V2);
	struct CallbackInfo {
		void *instance;
		CallbackType callback;

		CallbackInfo(void* instance, CallbackType callback) : instance(instance), callback(callback) {}
	};

	DelegateRef add(Key key, void* instance, CallbackType callback) {
		typename std::unordered_map<Key, CallbackInfo>::iterator it;
		it = callbacks.insert(std::pair<Key, CallbackInfo>(key, CallbackInfo(instance, callback) ));
		return (DelegateRef)&(it->second.callback);
	}

	void del(DelegateRef ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(Key key) {
		callbacks.erase(key);
	}

	void reserve(typename std::unordered_multimap<Key, CallbackInfo>::size_type n) {
		//callbacks.reserve(n); //not supported by msvc2010
	}

	void dispatch(Key key, V1 v1, V2 v2) {
		typedef typename std::unordered_map<Key, CallbackInfo>::const_iterator CallbackIterator;
		std::pair<CallbackIterator, CallbackIterator> callbackIterators = callbacks.equal_range(key);
		CallbackIterator it;

		for(it = callbackIterators.first; it != callbackIterators.second;) {
			const CallbackInfo& callbackInfo = it->second;

			if(callbackInfo.callback != nullptr) {
				callbackInfo.callback(callbackInfo.instance, v1, v2);
				++it;
			} else {
				it = callbacks.erase(it);
			}
		}
	}

private:
	std::unordered_multimap<Key, CallbackInfo> callbacks;
};

template<typename V1, typename V2 = void, typename V3 = void, typename V4 = void>
class IDelegate {
public:
	typedef void (*CallbackType)(void* instance, V1, V2, V3, V4);

	DelegateRef add(void* instance, CallbackType callback) {
		typename std::unordered_map<void*, CallbackType>::iterator it;
		it = callbacks.insert(std::pair<void*, CallbackType>(instance, callback)).first;
		return (DelegateRef)&it->second;
	}

	void del(DelegateRef ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(void* key) {
		callbacks.erase(key);
	}

	void dispatch(V1 v1, V2 v2, V3 v3, V4 v4) {
		typedef typename std::unordered_map<void*, CallbackType>::const_iterator CallbackIterator;
		CallbackIterator it, itEnd;

		for(it = callbacks.cbegin(), itEnd = callbacks.cend(); it != itEnd;) {
			void* instance = it->first;
			const CallbackType& callback = it->second;

			if(callback != nullptr) {
				callback(instance, v1, v2, v3, v4);
				++it;
			} else {
				it = callbacks.erase(it);
			}
		}
	}

private:
	std::unordered_map<void*, CallbackType> callbacks;
};

template<typename V1, typename V2, typename V3>
class IDelegate<V1, V2, V3, void> {
public:
	typedef void (*CallbackType)(void* instance, V1, V2, V3);

	DelegateRef add(void* instance, CallbackType callback) {
		typename std::unordered_map<void*, CallbackType>::iterator it;
		it = callbacks.insert(std::pair<void*, CallbackType>(instance, callback)).first;
		return (DelegateRef)&it->second;
	}

	void del(DelegateRef ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(void* key) {
		callbacks.erase(key);
	}

	void dispatch(V1 v1, V2 v2, V3 v3) {
		typedef typename std::unordered_map<void*, CallbackType>::const_iterator CallbackIterator;
		CallbackIterator it, itEnd;

		for(it = callbacks.cbegin(), itEnd = callbacks.cend(); it != itEnd;) {
			void* instance = it->first;
			const CallbackType& callback = it->second;

			if(callback != nullptr) {
				callback(instance, v1, v2, v3);
				++it;
			} else {
				it = callbacks.erase(it);
			}
		}
	}

private:
	std::unordered_map<void*, CallbackType> callbacks;
};

template<typename V1, typename V2>
class IDelegate<V1, V2, void, void> {
public:
	typedef void (*CallbackType)(void* instance, V1, V2);

	DelegateRef add(void* instance, CallbackType callback) {
		typename std::unordered_map<void*, CallbackType>::iterator it;
		it = callbacks.insert(std::pair<void*, CallbackType>(instance, callback)).first;
		return (DelegateRef)&it->second;
	}

	void del(DelegateRef ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(void* key) {
		callbacks.erase(key);
	}

	void dispatch(V1 v1, V2 v2) {
		typedef typename std::unordered_map<void*, CallbackType>::const_iterator CallbackIterator;
		CallbackIterator it, itEnd;

		for(it = callbacks.cbegin(), itEnd = callbacks.cend(); it != itEnd;) {
			void* instance = it->first;
			const CallbackType& callback = it->second;

			if(callback != nullptr) {
				callback(instance, v1, v2);
				++it;
			} else {
				it = callbacks.erase(it);
			}
		}
	}

private:
	std::unordered_map<void*, CallbackType> callbacks;
};

template<typename V1>
class IDelegate<V1, void, void, void> {
public:
	typedef void (*CallbackType)(void* instance, V1);

	DelegateRef add(void* instance, CallbackType callback) {
		typename std::unordered_map<void*, CallbackType>::iterator it;
		it = callbacks.insert(std::pair<void*, CallbackType>(instance, callback)).first;
		return (DelegateRef)&it->second;
	}

	void del(DelegateRef ptr) {
		if(ptr)
			*ptr = nullptr;
	}

	void del(void* key) {
		callbacks.erase(key);
	}

	void dispatch(V1 v1) {
		typedef typename std::unordered_map<void*, CallbackType>::const_iterator CallbackIterator;
		CallbackIterator it, itEnd;

		for(it = callbacks.cbegin(), itEnd = callbacks.cend(); it != itEnd;) {
			void* instance = it->first;
			const CallbackType& callback = it->second;

			if(callback != nullptr) {
				callback(instance, v1);
				++it;
			} else {
				it = callbacks.erase(it);
			}
		}
	}

private:
	std::unordered_map<void*, CallbackType> callbacks;
};

#endif

#endif // IDELEGATE_H
