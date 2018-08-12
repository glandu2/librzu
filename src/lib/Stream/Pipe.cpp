#include "Pipe.h"
#include <string.h>

Pipe::Pipe(uv_loop_t* uvLoop, bool logPackets) : Stream(uvLoop, (uv_stream_t*) &pipe, logPackets) {}

Pipe::~Pipe() {
	if(getState() != UnconnectedState)
		abort();
	while(getState() != UnconnectedState)
		uv_run(getLoop(), UV_RUN_ONCE);
}

StreamAddress Pipe::getAddress(int (*getsockaddrFunction)(const uv_pipe_t*, char*, size_t*)) {
	StreamAddress address{};
	char sockAddr[108] = {0};
	size_t sockAddrLen = sizeof(address.rawAddress);

	int ret = getsockaddrFunction(&pipe, sockAddr, &sockAddrLen);

	address.type = StreamAddress::ST_Pipe;

	if(ret >= 0) {
		address.pipeAddress = sockAddr;
	}

	return address;
}

StreamAddress Pipe::getRemoteAddress() {
	return getAddress(&uv_pipe_getpeername);
}

StreamAddress Pipe::getLocalAddress() {
	return getAddress(&uv_pipe_getsockname);
}

int Pipe::connect_impl(uv_connect_t* connectRequest, const std::string& hostName, uint16_t port) {
	uv_pipe_init(getLoop(), &pipe, false);

	uv_pipe_connect(connectRequest, &pipe, hostName.c_str(), &onConnected);

	return 0;
}

int Pipe::bind_impl(const std::string& interfaceIp, uint16_t port) {
	uv_pipe_init(getLoop(), &pipe, false);

	return uv_pipe_bind(&pipe, interfaceIp.c_str());
}

Stream* Pipe::createStream_impl() {
	Pipe* newStream = new Pipe(getLoop(), logPackets);
	uv_pipe_init(getLoop(), &newStream->pipe, false);

	return newStream;
}
