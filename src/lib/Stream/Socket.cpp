#include "Socket.h"
#include "Config/GlobalCoreConfig.h"
#include <string.h>

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

Stream* Socket::createStream_impl() {
	Socket* newStream = new Socket(getLoop(), logPackets);
	uv_tcp_init(getLoop(), &newStream->socket);

	return newStream;
}

void Socket::retrieveSocketBoundsInfo() {
	// Retrieve remote bound infos
	struct sockaddr_in sockInfo;
	int sockAddrLen = sizeof(sockaddr_in);
	if(uv_tcp_getpeername(&socket, (struct sockaddr*)&sockInfo, &sockAddrLen) >= 0) {
		if(remoteIp != sockInfo.sin_addr.s_addr) {
			remoteIp = sockInfo.sin_addr.s_addr;
			remoteIpStr[0] = 0;
		}
		remotePort = ntohs(sockInfo.sin_port);
	} else {
		remoteIpStr[0] = 0;
		remoteIp = 0;
		remotePort = 0;
	}

	// Retrieve local bound infos
	sockAddrLen = sizeof(sockaddr_in);
	if(uv_tcp_getsockname(&socket, (struct sockaddr*)&sockInfo, &sockAddrLen) >= 0) {
		if(localIp != sockInfo.sin_addr.s_addr) {
			localIp = sockInfo.sin_addr.s_addr;
			localIpStr[0] = 0;
		}
		localPort = ntohs(sockInfo.sin_port);
	} else {
		localIpStr[0] = 0;
		localIp = 0;
		localPort = 0;
	}

	setDirtyObjectName();
}

const char* Socket::getRemoteIpStr() {
	if(remoteIpStr[0] == '\0' && remoteIp)
		uv_inet_ntop(AF_INET, &remoteIp, remoteIpStr, sizeof(remoteIpStr));

	return remoteIpStr;
}

const char* Socket::getLocalIpStr() {
	if(localIpStr[0] == '\0' && localIp)
		uv_inet_ntop(AF_INET, &localIp, localIpStr, sizeof(localIpStr));

	return localIpStr;
}

void Socket::setKeepAlive(int delay) {
	uv_tcp_keepalive(&socket, delay > 0 ? true : false, delay);
}

void Socket::setNoDelay(bool enable) {
	uv_tcp_nodelay(&socket, enable);
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
	const char* localhostStr = getLocalIpStr();
	const char* remotehostStr = getRemoteIpStr();
	size_t nameSize = getClassNameSize() + strlen(localhostStr) + strlen(remotehostStr) + 19;
	setObjectName(nameSize, "%s[%s:%u <> %s:%u]", getClassName(), localhostStr, localPort, remotehostStr, remotePort);
}

