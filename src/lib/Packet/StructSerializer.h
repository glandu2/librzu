#pragma once

#include "../Extern.h"
#include "GameTypes.h"
#include "PacketEpics.h"
#include <type_traits>
#include <vector>

class RZU_EXTERN StructSerializer {
private:
	packet_version_t version;

public:
	StructSerializer(packet_version_t version) : version(version) {}

	packet_version_t getVersion() const { return version; }

	// Type checking /////////////////////////

	// Primitives
	template<typename T>
	struct is_strong_typed_primitive
	    : public std::integral_constant<bool,
	                                    std::is_same<T, ar_handle_t>::value || std::is_same<T, ar_time_t>::value> {};

	template<typename T>
	struct is_primitive : public std::integral_constant<bool,
	                                                    std::is_fundamental<T>::value || std::is_enum<T>::value ||
	                                                        is_strong_typed_primitive<T>::value> {};

	// Primitives with cast
	template<typename T, typename U>
	struct is_castable_primitive : public std::integral_constant<bool,
	                                                             is_primitive<T>::value && is_primitive<U>::value &&
	                                                                 !std::is_same<T, U>::value> {};
};
