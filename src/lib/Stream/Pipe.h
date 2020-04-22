#pragma once

#include "Stream.h"
#include "uv.h"
#include <stdint.h>
#include <string>

class RZU_EXTERN Pipe : public Stream {
	DECLARE_CLASS(Pipe)

public:
public:
	Pipe(uv_loop_t* uvLoop, bool logPackets = true);
	virtual ~Pipe();

	virtual StreamAddress getRemoteAddress();
	virtual StreamAddress getLocalAddress();

protected:
	virtual int connect_impl(uv_connect_t* connectRequest, const std::string& hostName, uint16_t port);
	virtual int bind_impl(const std::string& interfaceIp, uint16_t port);
	virtual Stream* createStream_impl();

	StreamAddress getAddress(int (*getsockaddrFunction)(const uv_pipe_t* handle, char* buffer, size_t* size));

private:
	uv_pipe_t pipe;
};

