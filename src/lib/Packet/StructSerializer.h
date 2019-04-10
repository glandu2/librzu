#ifndef PACKET_STRUCTSERIALIZER_H
#define PACKET_STRUCTSERIALIZER_H

#include "../Extern.h"
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
	struct is_primitive : public std::integral_constant<bool, std::is_fundamental<T>::value || std::is_enum<T>::value> {
	};

	// Primitives with cast
	template<typename T, typename U>
	struct is_castable_primitive : public std::integral_constant<bool,
	                                                             is_primitive<T>::value && is_primitive<U>::value &&
	                                                                 !std::is_same<T, U>::value> {};
};

#endif  // PACKET_STRUCTSERIALIZER_H
