#include "Socket.h"

#include "Log.h"
#include <sstream>
#include <iomanip>
#include <stdarg.h>
#include <string.h>
#include "RappelzLibConfig.h"

Socket::Socket(uv_loop_t *uvLoop, bool logPackets)
	: Stream(uvLoop, (uv_stream_t*)&socket, logPackets)
{
}

Socket::~Socket() {
	if(getState() != UnconnectedState)
		abort();
	while(getState() != UnconnectedState)
		uv_run(getLoop(), UV_RUN_ONCE);
}

int Socket::connect_impl(uv_connect_t* connectRequest, const std::string & hostName, uint16_t port) {
	uv_tcp_init(getLoop(), &socket);

	struct sockaddr_in dest;

	uv_ip4_addr(hostName.c_str(), port, &dest);
	return uv_tcp_connect(connectRequest, &socket, (struct sockaddr*)&dest, &onConnected);
}

int Socket::bind_impl(const std::string& interfaceIp, uint16_t port) {
	uv_tcp_init(getLoop(), &socket);

	struct sockaddr_in bindAddr;

	uv_ip4_addr(interfaceIp.c_str(), port, &bindAddr);
	return uv_tcp_bind(&socket, (struct sockaddr*)&bindAddr, 0);
}

void Socket::createStream_impl() {
	uv_tcp_init(getLoop(), &socket);
}

void Socket::retrieveSocketBoundsInfo() {
	// Retrieve remote bound infos
	struct sockaddr_in sockInfo;
	int sockAddrLen = sizeof(sockaddr_in);
	if(uv_tcp_getpeername(&socket, (struct sockaddr*)&sockInfo, &sockAddrLen) >= 0) {
		if(remoteHost != sockInfo.sin_addr.s_addr) {
			remoteHost = sockInfo.sin_addr.s_addr;
			remoteHostName[0] = 0;
		}
		remotePort = ntohs(sockInfo.sin_port);
	} else {
		remoteHostName[0] = 0;
		remoteHost = 0;
		remotePort = 0;
	}

	// Retrieve local bound infos
	sockAddrLen = sizeof(sockaddr_in);
	if(uv_tcp_getsockname(&socket, (struct sockaddr*)&sockInfo, &sockAddrLen) >= 0) {
		if(localHost != sockInfo.sin_addr.s_addr) {
			localHost = sockInfo.sin_addr.s_addr;
			localHostName[0] = 0;
		}
		localPort = ntohs(sockInfo.sin_port);
	} else {
		localHostName[0] = 0;
		localHost = 0;
		localPort = 0;
	}

	setDirtyObjectName();
}

const char* Socket::getRemoteHostName() {
	if(remoteHostName[0] == '\0' && remoteHost)
		uv_inet_ntop(AF_INET, &remoteHost, remoteHostName, sizeof(remoteHostName));

	return remoteHostName;
}

const char* Socket::getLocalHostName() {
	if(localHostName[0] == '\0' && localHost)
		uv_inet_ntop(AF_INET, &localHost, localHostName, sizeof(localHostName));

	return localHostName;
}

void Socket::onStateChanged(State oldState, State newState) {
	if(newState == ConnectedState) {
		if(CONFIG_GET()->app.useTcpNoDelay.get())
			uv_tcp_nodelay(&socket, true);
	}
}

void Socket::updateObjectName() {
//	int nameSize = 0;
//	nameSize += getClassNameSize(); //name prefix is class name
//	nameSize += 1; // "["
//	nameSize += strlen(localHostName);
//	nameSize += 1; // ":"
//	nameSize += 5; // localPort
//	nameSize += 4; // " <> "
//	nameSize += strlen(remoteHostName);
//	nameSize += 1; // ":"
//	nameSize += 5; // remotePort
//	nameSize += 2; // "]\0"
	const char* localhostStr = getLocalHostName();
	const char* remotehostStr = getRemoteHostName();
	int nameSize = getClassNameSize() + strlen(localhostStr) + strlen(remotehostStr) + 19;
	setObjectName(nameSize, "%s[%s:%u <> %s:%u]", getClassName(), localhostStr, localPort, remotehostStr, remotePort);
}
