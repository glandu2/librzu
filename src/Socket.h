#ifndef SOCKET_H
#define SOCKET_H
/*
#include "Pipe.h"

#define Socket Pipe*/

#include "Stream.h"
#include "uv.h"
#include "IListener.h"
#include "stdint.h"
#include "IDelegate.h"
#include "Log.h"

#include <unordered_map>
#include <string>
#include <vector>

class RZU_EXTERN Socket : public Stream
{
	DECLARE_CLASS(Socket)
public:
	Socket(uv_loop_t* uvLoop, bool logPackets = true);
	virtual ~Socket();

	const char* getRemoteIpStr();
	const char* getLocalIpStr();

	void setKeepAlive(int delay);

protected:
	virtual int connect_impl(uv_connect_t* connectRequest, const std::string & hostName, uint16_t port);
	virtual int bind_impl(const std::string& interfaceIp, uint16_t port);
	virtual Stream *createStream_impl();

	virtual void retrieveSocketBoundsInfo();
	virtual void updateObjectName();

	virtual void onStateChanged(State oldState, State newState);

private:
	uv_tcp_t socket;
};

#endif // SOCKET_H
