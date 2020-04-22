#pragma once

#include "PacketEpics.h"
#include <stdint.h>

template<class Encoder> class EncodedInt {
public:
	EncodedInt() = default;
	EncodedInt(uint32_t v) : value(v) {}
	EncodedInt(const EncodedInt& v) = default;
	EncodedInt& operator=(const EncodedInt& v) = default;

	EncodedInt operator=(uint32_t v) {
		value = v;
		return *this;
	}
	bool operator==(uint32_t v) const { return v == value; }
	bool operator==(EncodedInt v) const { return v.value == value; }

	operator uint32_t() const { return value; }

	uint32_t getSize(packet_version_t version) const { return Encoder::getSize(version); }

	template<class T> void serialize(T* buffer) const { Encoder::template serialize<T>(buffer, value); }

	template<class T> void deserialize(T* buffer) { Encoder::template deserialize<T>(buffer, value); }

private:
	uint32_t value;
};
