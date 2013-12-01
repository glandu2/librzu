#ifndef SOCKET_H
#define SOCKET_H

#include "Interfaces/ISocket.h"

struct SocketInternal;
class SocketPoll;

class Socket : public CImplement<ISocket>
{
public:
	Socket();
	virtual ~Socket();

	void IFACECALLCONV deleteLater();

	bool IFACECALLCONV connect(const std::string& hostName, uint16_t port);
	bool IFACECALLCONV listen(const std::string& interfaceIp, u_int16_t port);

	size_t IFACECALLCONV read(void *buffer, size_t size);
	size_t IFACECALLCONV write(const void *buffer, size_t size);
	bool IFACECALLCONV accept(ISocket *socket);

	void IFACECALLCONV close();
	void IFACECALLCONV abort();

	State IFACECALLCONV getState();
	size_t IFACECALLCONV getAvailableBytes();

	DelegateRef IFACECALLCONV addDataListener(void* instance, CallbackOnDataReady listener);
	DelegateRef IFACECALLCONV addConnectionListener(void* instance, CallbackOnDataReady listener);
	DelegateRef IFACECALLCONV addEventListener(void* instance, CallbackOnStateChanged listener);
	DelegateRef IFACECALLCONV addErrorListener(void* instance, CallbackOnError listener);
	void IFACECALLCONV removeListener(void* instance);

	int64_t getFd();
	unsigned int getLastError();

	void notifyReadyRead();
	void notifyReadyWrite();
	void notifyReadyError(int errorValue);

	static void setPoll(SocketPoll* socketPoll);

protected:
	void IFACECALLCONV setState(State state);

private:
	SocketInternal *_p;
	int lastError;
	static SocketPoll* socketPoll;
};

#endif // SOCKET_H
