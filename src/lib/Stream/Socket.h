#ifndef SOCKET_H
#define SOCKET_H

#include "Stream.h"
#include "uv.h"
#include <stdint.h>
#include <string>

class RZU_EXTERN Socket : public Stream {
	DECLARE_CLASS(Socket)
public:
	Socket(uv_loop_t* uvLoop, bool logPackets = true);
	virtual ~Socket();

	virtual StreamAddress getRemoteAddress();
	virtual StreamAddress getLocalAddress();

	void setKeepAlive(int delay);
	void setNoDelay(bool enable);

protected:
	virtual int connect_impl(uv_connect_t* connectRequest, const std::string& hostName, uint16_t port);
	virtual int bind_impl(const std::string& interfaceIp, uint16_t port);
	virtual Stream* createStream_impl();

	virtual void updateObjectName();

	virtual void onStateChanged(State oldState, State newState);

	StreamAddress getAddress(int (*getsockaddrFunction)(const uv_tcp_t* handle,
	                                                struct sockaddr* name,
							                        int* namelen) );

private:
	static void enableFastPath(uv_tcp_t* socket);

private:
	uv_tcp_t socket;
};

#endif  // SOCKET_H
