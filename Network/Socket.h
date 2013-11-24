#ifndef SOCKET_H
#define SOCKET_H

#include "ISocket.h"

struct SocketInternal;
class SocketPoll;

class Socket : public CImplement<ISocket>
{
public:
	Socket();
	virtual ~Socket();

	bool IFACECALLCONV connect(const std::string & hostName, uint16_t port);
	size_t IFACECALLCONV read(void *buffer, size_t size);
	size_t IFACECALLCONV write(const void *buffer, size_t size);
	void IFACECALLCONV close();
	void IFACECALLCONV abort();
	State IFACECALLCONV getState();
	size_t IFACECALLCONV getAvailableBytes();

	void IFACECALLCONV addDataListener(void* instance, CallbackOnDataReady listener);
	void IFACECALLCONV addEventListener(void* instance, CallbackOnStateChanged listener);
	void IFACECALLCONV addErrorListener(void* instance, CallbackOnError listener);
	void IFACECALLCONV removeListener(void* instance);

	int64_t getFd();
	unsigned int getLastError();

	void notifyReadyRead();
	void notifyReadyWrite();
	void notifyReadyError();

	static void setPoll(SocketPoll* socketPoll);

protected:
	void IFACECALLCONV setState(State state);

private:
	SocketInternal *_p;
	static SocketPoll* socketPoll;
};

#endif // SOCKET_H
