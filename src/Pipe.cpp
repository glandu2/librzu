#include "Pipe.h"
#include "Log.h"
#include <sstream>
#include <iomanip>
#include <stdarg.h>
#include <string.h>
#include "RappelzLibConfig.h"
#include "EventLoop.h"

Pipe::Pipe(uv_loop_t *uvLoop, bool logPackets)
	: Stream(uvLoop, (uv_stream_t*)&pipe, logPackets)
{
}

Pipe::~Pipe() {
	if(getState() != UnconnectedState)
		abort();
	while(getState() != UnconnectedState)
		uv_run(getLoop(), UV_RUN_ONCE);
}

int Pipe::connect_impl(uv_connect_t* connectRequest, const std::string & hostName, uint16_t port) {
	uv_pipe_init(getLoop(), &pipe, false);

	uv_pipe_connect(connectRequest, &pipe, hostName.c_str(), &onConnected);

	return 0;
}

int Pipe::bind_impl(const std::string& interfaceIp, uint16_t port) {
	uv_pipe_init(getLoop(), &pipe, false);

	warn("Bind on %s\n", interfaceIp.c_str());
	return uv_pipe_bind(&pipe, interfaceIp.c_str());
}

void Pipe::createStream_impl() {
	uv_pipe_init(getLoop(), &pipe, false);
}
