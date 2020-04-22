#pragma once

#include <functional>
#include <stdint.h>

template<class Tag, typename T> class strong_typedef {
public:
	using underlying_type = T;

	strong_typedef() : value_() {}

	explicit strong_typedef(const T& value) : value_(value) {}

	explicit operator T&() noexcept { return value_; }

	operator const T&() const noexcept { return value_; }

	T get() const noexcept { return value_; }

	T& getRef() noexcept { return value_; }

private:
	T value_;
};

template<class T> class strong_typedef_addition {
public:
	friend T operator+(T a, T b) noexcept { return static_cast<T>(a.get() + b.get()); }
	friend T operator-(T a, T b) noexcept { return static_cast<T>(a.get() - b.get()); }
	friend T& operator+=(T& a, T b) noexcept {
		a.getRef() += b.get();
		return a;
	}
	friend T& operator-=(T& a, T b) noexcept {
		a.getRef() -= b.get();
		return a;
	}
};

struct ar_handle_t : strong_typedef<ar_handle_t, uint32_t> {
	using strong_typedef::strong_typedef;
};

struct ar_time_t : public strong_typedef<ar_time_t, uint32_t> {
	using strong_typedef::strong_typedef;
};

namespace std {
template<> struct hash<ar_handle_t> : public hash<uint32_t> {};
}  // namespace std

