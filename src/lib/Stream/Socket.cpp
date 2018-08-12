#include "Socket.h"
#include "Config/GlobalCoreConfig.h"
#include <string.h>

#ifndef SIO_LOOPBACK_FAST_PATH
#define SIO_LOOPBACK_FAST_PATH 0x98000010
#endif

#ifdef _WIN32
#if defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
typedef struct _OSVERSIONINFOW RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
#endif
typedef NTSTATUS(NTAPI* sRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);

/* Get the windows version once, used to test fastpath feature compatibility */
static uv_once_t fastpath_unsupported_guard = UV_ONCE_INIT;
static BOOL fastpath_supported = FALSE;
static BOOL fastpath_default_enable = FALSE;

static void init_fastpath_support(void) {
	HMODULE ntdll_module;
	sRtlGetVersion pRtlGetVersion;
	RTL_OSVERSIONINFOW osvi = {0};

	ntdll_module = GetModuleHandleA("ntdll.dll");
	if(!ntdll_module)
		return;

	pRtlGetVersion = (sRtlGetVersion) GetProcAddress(ntdll_module, "RtlGetVersion");
	if(!pRtlGetVersion)
		return;

	osvi.dwOSVersionInfoSize = sizeof(osvi);
	if(pRtlGetVersion(&osvi) == 0) {
		/* TCP fast path unsupported if < Windows 6.2 */
		fastpath_supported = osvi.dwMajorVersion > 6 || (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 2);

		fastpath_default_enable = fastpath_supported;
	}
}
#endif

Socket::Socket(uv_loop_t* uvLoop, bool logPackets) : Stream(uvLoop, (uv_stream_t*) &socket, logPackets) {}

Socket::~Socket() {
	if(getState() != UnconnectedState)
		abort();
	while(getState() != UnconnectedState)
		uv_run(getLoop(), UV_RUN_ONCE);
}

StreamAddress Socket::getAddress(int (*getsockaddrFunction)(const uv_tcp_t*, sockaddr*, int*)) {
	StreamAddress address{};
	union {
		struct sockaddr common;
		struct sockaddr_in ipv4;
		struct sockaddr_in6 ipv6;
	} sockAddr{};
	int sockAddrLen = sizeof(sockAddr);

	int ret = getsockaddrFunction(&socket, &sockAddr.common, &sockAddrLen);

	if(ret < 0) {
		address.type = StreamAddress::ST_SocketIpv4;
		memset(&address.rawAddress, 0, sizeof(address.rawAddress));
	} else if(sockAddr.common.sa_family == AF_INET6) {
		address.type = StreamAddress::ST_SocketIpv6;
		address.port = ntohs(sockAddr.ipv6.sin6_port);
		address.rawAddress.ipv6 = sockAddr.ipv6.sin6_addr;
	} else {
		address.type = StreamAddress::ST_SocketIpv4;
		address.port = ntohs(sockAddr.ipv4.sin_port);
		address.rawAddress.ipv4 = sockAddr.ipv4.sin_addr;
	}

	return address;
}

StreamAddress Socket::getRemoteAddress() {
	return getAddress(&uv_tcp_getpeername);
}

StreamAddress Socket::getLocalAddress() {
	return getAddress(&uv_tcp_getsockname);
}

int Socket::connect_impl(uv_connect_t* connectRequest, const std::string& hostName, uint16_t port) {
	struct sockaddr_in6 dest6;
	struct sockaddr_in dest4;
	struct sockaddr* dest = nullptr;

	if(hostName.find(':') != std::string::npos) {
		uv_tcp_init_ex(getLoop(), &socket, AF_INET6);

		uv_ip6_addr(hostName.c_str(), port, &dest6);
		dest = (struct sockaddr*) &dest6;
	} else {
		uv_tcp_init_ex(getLoop(), &socket, AF_INET);

		uv_ip4_addr(hostName.c_str(), port, &dest4);
		dest = (struct sockaddr*) &dest4;

		/* loopback in IPv4 is 127.0.0.0/8 */
		if((dest4.sin_addr.s_addr & 0xFF) == 127)
			enableFastPath(&socket);
	}

	return uv_tcp_connect(connectRequest, &socket, dest, &onConnected);
}

int Socket::bind_impl(const std::string& interfaceIp, uint16_t port) {
	struct sockaddr_in6 bindAddr6;
	struct sockaddr_in bindAddr4;
	struct sockaddr* bindAddr;

	if(interfaceIp.find(':') != std::string::npos) {
		uv_tcp_init_ex(getLoop(), &socket, AF_INET6);

		uv_ip6_addr(interfaceIp.c_str(), port, &bindAddr6);
		bindAddr = (struct sockaddr*) &bindAddr6;
	} else {
		uv_tcp_init_ex(getLoop(), &socket, AF_INET);

		uv_ip4_addr(interfaceIp.c_str(), port, &bindAddr4);
		bindAddr = (struct sockaddr*) &bindAddr4;

#ifdef _WIN32
		enableFastPath(&socket);
#endif
	}

	return uv_tcp_bind(&socket, bindAddr, 0);
}

void Socket::enableFastPath(uv_tcp_t* socket) {
#ifdef _WIN32
	uv_os_fd_t socketFd;
	DWORD bytes;
	DWORD enable;

	uv_once(&fastpath_unsupported_guard, &init_fastpath_support);

	if(!fastpath_supported)
		return;

	if(uv_fileno((uv_handle_t*) &socket, &socketFd))
		return;

	enable = 1;
	WSAIoctl((SOCKET) socketFd, SIO_LOOPBACK_FAST_PATH, &enable, sizeof(enable), NULL, 0, &bytes, NULL, NULL);
#endif
}

Stream* Socket::createStream_impl() {
	Socket* newStream = new Socket(getLoop(), logPackets);
	uv_tcp_init(getLoop(), &newStream->socket);

	return newStream;
}

void Socket::setKeepAlive(int delay) {
	uv_tcp_keepalive(&socket, delay > 0 ? true : false, delay);
}

void Socket::setNoDelay(bool enable) {
	uv_tcp_nodelay(&socket, enable);
}

void Socket::onStateChanged(State oldState, State newState) {
	if(newState == ConnectedState) {
		if(GlobalCoreConfig::get()->app.useTcpNoDelay.get())
			uv_tcp_nodelay(&socket, true);
	}
}

void Socket::updateObjectName() {
	//	int nameSize = 0;
	//	nameSize += getClassNameSize(); //name prefix is class name
	//	nameSize += 2; // "[["
	//	nameSize += strlen(localHostName);
	//	nameSize += 2; // "]:"
	//	nameSize += 5; // localPort
	//	nameSize += 4; // " <> "
	//	nameSize += strlen(remoteHostName);
	//	nameSize += 2; // ":["
	//	nameSize += 5; // remotePort
	//	nameSize += 3; // "]]\0"
	char localhostStr[256];
	char remotehostStr[256];

	StreamAddress localAddress = getLocalAddress();
	StreamAddress remoteAddress = getRemoteAddress();

	localAddress.getName(localhostStr, sizeof(localhostStr));
	remoteAddress.getName(remotehostStr, sizeof(localhostStr));

	size_t nameSize = getClassNameSize() + strlen(localhostStr) + strlen(remotehostStr) + 23;
	setObjectName(nameSize,
	              "%s([%s]:%u <> [%s]:%u)",
	              getClassName(),
	              localhostStr,
	              localAddress.port,
	              remotehostStr,
	              remoteAddress.port);
}
