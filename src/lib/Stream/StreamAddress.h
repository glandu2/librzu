#ifndef STREAMADDRESS_H
#define STREAMADDRESS_H

#include "../Extern.h"
#include "uv.h"
#include <string>

class RZU_EXTERN StreamAddress {
public:
	enum AddressType { ST_SocketIpv4, ST_SocketIpv6, ST_Pipe };
	union RawAddressUnion {
		struct in_addr ipv4;
		struct in6_addr ipv6;
	};

public:
	int getName(char* outStr, int maxSize) const;
	bool operator==(const StreamAddress& other) const;

	AddressType type;
	RawAddressUnion rawAddress;
	std::string pipeAddress;
	uint16_t port;
};

struct RZU_EXTERN StreamAddressHasher {
	size_t operator()(const StreamAddress& k) const;
};

#endif  // STREAM_H
