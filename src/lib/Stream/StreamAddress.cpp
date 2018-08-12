#include "StreamAddress.h"
#include <string.h>

int StreamAddress::getName(char* outStr, int maxSize) const {
	switch(type) {
		case ST_SocketIpv4:
			uv_inet_ntop(AF_INET, &rawAddress.ipv4, outStr, maxSize);
			break;
		case ST_SocketIpv6:
			uv_inet_ntop(AF_INET6, &rawAddress.ipv6, outStr, maxSize);
			break;
		case ST_Pipe:
			strncpy(outStr, pipeAddress.c_str(), maxSize);
			break;
	}

	return 0;
}

int StreamAddress::setFromName(const char* ipStr) {
	if(strncmp(ipStr, "pipe:", 5) == 0) {
		type = StreamAddress::ST_Pipe;
		pipeAddress = std::string(ipStr + 5);
		return 0;
	} else if(strchr(ipStr, ':')) {
		type = StreamAddress::ST_SocketIpv6;
		return uv_inet_pton(AF_INET6, ipStr, &rawAddress.ipv6);
	} else {
		type = StreamAddress::ST_SocketIpv4;
		return uv_inet_pton(AF_INET, ipStr, &rawAddress.ipv4);
	}
}

bool StreamAddress::operator==(const StreamAddress& other) const {
	if(type != other.type || port != other.port)
		return false;

	switch(type) {
		case ST_SocketIpv4:
			return memcmp(&rawAddress.ipv4, &other.rawAddress.ipv4, sizeof(rawAddress.ipv4)) == 0;
		case ST_SocketIpv6:
			return memcmp(&rawAddress.ipv6, &other.rawAddress.ipv6, sizeof(rawAddress.ipv6)) == 0;
		case ST_Pipe:
			return pipeAddress == other.pipeAddress;
	}

	return true;
}

size_t StreamAddressHasher::operator()(const StreamAddress& k) const {
	size_t hash;

	hash = std::hash<uint32_t>()(k.type + (k.port << 8));

	switch(k.type) {
		case StreamAddress::ST_SocketIpv4:
			hash ^= std::hash<uint32_t>()(k.rawAddress.ipv4.s_addr) << 1;
			break;
		case StreamAddress::ST_SocketIpv6: {
			static_assert(sizeof(uint64_t) * 2 == sizeof(k.rawAddress.ipv6), "IPv6 address size must be 2 * uint64_t");
			const uint64_t* rawIp = reinterpret_cast<const uint64_t*>(&k.rawAddress.ipv6);
			hash ^= std::hash<uint64_t>()(rawIp[0]) << 1;
			hash ^= std::hash<uint64_t>()(rawIp[1]) << 2;
			break;
		}
		case StreamAddress::ST_Pipe:
			hash ^= std::hash<std::string>()(k.pipeAddress) << 1;
			break;
	}
	return hash;
}
