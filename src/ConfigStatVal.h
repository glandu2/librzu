#ifndef CONFIGSTATVAL_H
#define CONFIGSTATVAL_H

#include "ConfigValue.h"

#ifdef _WIN32
#include <Windows.h>

template<typename T>
class cstatval
{
public:
	cstatval(T initVal = 0) : std::atomic<T>(initVal) {}

	T get() { return (T) InterlockedCompareExchange(&value, 0, 0); }
	T get(const T& def) { return (T) InterlockedCompareExchange(&value, 0, 0); }
	void set(const T& val, bool dispatch = true) { InterlockedExchange(&value, (unsigned long long) val); }

	T operator++(int) { return InterlockedIncrement(&value); }
	T operator--(int) { return InterlockedDecrement(&value); }

	T operator++() { return InterlockedIncrement(&value) + 1; }
	T operator--() { return InterlockedDecrement(&value) - 1; }

	cstatval<T>& operator= (T val) { set(val); return *this; }

public:
	__declspec (align(8)) volatile unsigned long long value;
};

#else // _WIN32

#include <atomic>

template<typename T>
class cstatval : public ConfigTypedValue<T>
{
public:
	cstatval(T initVal = 0) : value(initVal) {}

	T get() { return value.load(); }
	T get(const T& def) { return value.load(); }
	void set(const T& val, bool dispatch = true) { value.store(val); }

	T operator++(int) { return value++; }
	T operator--(int) { return value--; }

	T operator++() { return ++value; }
	T operator--() { return --value; }

	cstatval<T>& operator= (T val) { set(val); return *this; }

private:
	std::atomic<T> value;
};
#endif // _WIN32

#endif // CONFIGSTATVAL_H
